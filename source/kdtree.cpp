#include "kdtree.h"

#include "aabb.h"
#include "geometry.h"
#include "math.h"
#include "mesh.h"
#include "plane.h"
#include <algorithm>

struct BuildPartition;
struct BuildLeaf;

struct BuildNode
{
	BuildNode(bool leaf)
	  : leaf(leaf)
	{
	}
	bool leaf;

	explicit operator BuildPartition*()
	{
		return leaf ? nullptr : (struct BuildPartition*)this;
	}

	explicit operator BuildLeaf*()
	{
		return leaf ? nullptr : (struct BuildLeaf*)this;
	}
};

struct BuildPartition : public BuildNode
{
	BuildPartition()
	  : BuildNode(false)
	{
	}
	int axis;
	size_t firstChild;
	size_t secondChild;
};

struct BuildLeaf : public BuildNode
{
	Aabb aabb;

	struct Triangle
	{
		size_t position[3];
		size_t normal[3];
		size_t textureCoordinates[3];
	};

	std::vector<Triangle> triangles;
	const std::vector<Vec3f>& positions;

	Triangle buildTri;
	size_t buildTriState[3];

	BuildLeaf(const std::vector<Vec3f>& positions)
	  : BuildNode(true)
	  , positions(positions)
	{
		resetBuildTri();
	}

	BuildLeaf(BuildLeaf& source)
	  : BuildNode(true)
	  , positions(source.positions)
	{
	}

	void resetBuildTri()
	{
		memset(&buildTri, 0xff, sizeof(Triangle));
		memset(&buildTriState, 0, sizeof(size_t) * 3);
	}

	void pushPosition(size_t i)
	{
		buildTri.position[buildTriState[0]] = i;
		buildTriState[0]++;
	}

	void pushNormal(size_t i)
	{
		buildTri.normal[buildTriState[1]] = i;
		buildTriState[1]++;
	}

	void pushTextureCoordinate(size_t i)
	{
		buildTri.textureCoordinates[buildTriState[2]] = i;
		buildTriState[2]++;
	}

	void flush()
	{
		addTriangle(buildTri);
		resetBuildTri();
	}

	void addTriangle(const Triangle& tri)
	{
		triangles.push_back(tri);
		for (size_t i = 0; i < 3; ++i)
		{
			aabb.grow(positions[tri.position[i]]);
		}
	}

	static void split(BuildLeaf& source, int splitAxis, float splitDistance, BuildLeaf& first, BuildLeaf& second)
	{
		Vec3f N(0.0f);
		N[splitAxis] = 1.0f;
		Plane p(N, splitDistance);

		for (auto const& tri : source.triangles)
		{
			int classification =
			  geometry::classifyTriangle(source.positions[tri.position[0]], source.positions[tri.position[1]],
			                             source.positions[tri.position[2]], p);

			if (classification <= 0)
			{
				first.addTriangle(tri);
			}

			if (classification >= 0)
			{
				second.addTriangle(tri);
			}
		}
	}
};

void KdTree::build(const Mesh& mesh)
{
	m_positions = mesh.m_positions;
	m_normals = mesh.m_normals;
	m_textureCoordinates = mesh.m_textureCoordinates;

	BuildLeaf root(m_positions);

	for (const auto& microMesh : mesh.m_microMeshes)
	{
		for (size_t index = 0; index < microMesh.m_indices.size();)
		{
			for (size_t i = 0; i < 3; ++i, ++index)
			{
				const Mesh::Vertex& vertex = mesh.m_vertices.at(microMesh.m_indices[index]);
				root.pushPosition(vertex.m_position);
				root.pushNormal(vertex.m_normal);
				root.pushTextureCoordinate(vertex.m_textureCoordinates);
			}
			root.flush();
		}
	}

	m_aabb = root.aabb;

	m_nodes.push_back(Node());
	Node& node = m_nodes.at(m_nodes.size() - 1);
	buildRecurse(root, node, 0);
}

bool KdTree::intersect(const Ray& ray, float& t) const
{
	float tMin = std::numeric_limits<float>::epsilon();
	float tMax = std::numeric_limits<float>::max();
	return intersectRecurse(ray, 0, m_aabb, tMin, tMax, t);
}

void KdTree::getSurfaceData(HitData& hitData) const
{
}

bool shouldSplit(BuildLeaf& node, int& splitAxis, float& splitDistance)
{
	if (node.triangles.size() < 101)
	{
		return false;
	}

	float maxExtent = std::numeric_limits<float>::min();

	for (int i = 0; i < 3; ++i)
	{
		float extent = node.aabb.m_maxs[i] - node.aabb.m_mins[i];
		if (extent > maxExtent)
		{
			maxExtent = extent;
			splitAxis = i;
		}
	}

	if (maxExtent < 0.05f)
	{
		return false;
	}

	splitDistance = node.aabb.m_mins[splitAxis] + maxExtent * 0.5f;

	return true;
}

void KdTree::buildRecurse(BuildLeaf& leaf, Node& node, int depth)
{
	int splitAxis;
	float splitDistance;
	if (depth < 16 && shouldSplit(leaf, splitAxis, splitDistance))
	{
		size_t firstChild = m_nodes.size();
		node.m_inner.setData(splitAxis, splitDistance, firstChild);
		m_nodes.push_back(Node());
		m_nodes.push_back(Node());

		BuildLeaf firstLeaf(leaf);
		BuildLeaf secondLeaf(leaf);
		BuildLeaf::split(leaf, splitAxis, splitDistance, firstLeaf, secondLeaf);

		buildRecurse(firstLeaf, m_nodes.at(firstChild), depth + 1);
		buildRecurse(secondLeaf, m_nodes.at(firstChild + 1), depth + 1);
	}
	else
	{
		node.m_leaf.m_startIndex = m_triangles.size();
		node.m_leaf.m_length = leaf.triangles.size();

		for (const auto& leafTriangle : leaf.triangles)
		{
			Triangle tri;

			for (size_t i = 0; i < 3; ++i)
			{
				tri.m_positions[i] = leafTriangle.position[i];
				tri.m_normals[i] = leafTriangle.normal[i];
				tri.m_textureCoordinates[i] = leafTriangle.textureCoordinates[i];
			}

			m_triangles.push_back(tri);
		}
	}
}

bool triangle_intersection(const Vec3f& V1, const Vec3f& V2, const Vec3f& V3, const Ray& ray, float& t, float& u,
                           float& v);

bool KdTree::intersectRecurse(const Ray& ray, size_t node, Aabb aabb, float tMin, float tMax, float& hitT) const
{
	const Node& n = m_nodes.at(node);
	int axis;
	float splitDistance;
	size_t firstChild;

	if (n.m_inner.getData(axis, splitDistance, firstChild))
	{
		float firstT = std::numeric_limits<float>::max();
		Aabb firstAabb(aabb);
		firstAabb.m_maxs[axis] = splitDistance;	
		bool first = intersectRecurse(ray, firstChild, aabb, tMin, tMax, firstT);

		float secondT = std::numeric_limits<float>::max();
		Aabb secondAabb(aabb);
		secondAabb.m_mins[axis] = splitDistance;
		bool second = intersectRecurse(ray, firstChild + 1, aabb, tMin, tMax, secondT);

		if (first && second)
		{
			hitT = std::min(firstT, secondT);
		}
		else if (first)
		{
			hitT = firstT;
			return true;
		}
		else if (second)
		{
			hitT = secondT;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < n.m_leaf.m_length; ++i)
		{
			const Triangle& tri = m_triangles.at(n.m_leaf.m_startIndex + i);
			float t, u, v;
			if (triangle_intersection(m_positions[tri.m_positions[0]], m_positions[tri.m_positions[1]],
			                          m_positions[tri.m_positions[2]], ray, t, u, v)
			    && (compareFloats(t, tMin) >= 0) && (compareFloats(t, tMax) <= 0) && t < hitT)
			{
				hitT = t;
			}
		}
	}

	return (hitT < std::numeric_limits<float>::max());
}
