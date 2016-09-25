#pragma once

#include "aabb.h"
#include "renderable.h"

#include <cassert>
#include <vector>

class Mesh;
class Vec3f;

class KdTree : public IRenderable
{
public:
	void build(const Mesh& mesh);

	bool intersect(const Ray& ray, float& t) const;
	void getSurfaceData(HitData& hitData) const;

private:
	struct InnerNode
	{
		size_t m_nodeData;
		float m_distance;

		void setData(int axis, float splitDistance, size_t firstChild)
		{
			assert((firstChild & 0x1fff) == firstChild);
			m_nodeData = ((1 << 2) | axis) << (29);
			m_nodeData |= (firstChild & 0x1fff);
			m_distance = splitDistance;
		}

		bool getData(int& axis, float& splitDistance, size_t& firstChild) const
		{
			bool innerNode = ((m_nodeData >> (sizeof(size_t) * 8 - 1)) == 1);
			if (innerNode)
			{
				axis = (m_nodeData >> (sizeof(size_t) * 8 - 3)) & 3;
				firstChild = m_nodeData & ~(7 << (sizeof(size_t) * 8 - 3));
				splitDistance = m_distance;
			}
			return innerNode;
		}
	};

	struct LeafNode
	{
		size_t m_startIndex;
		size_t m_length;
	};

	union Node {
		InnerNode m_inner;
		LeafNode m_leaf;
	};

	struct Triangle
	{
		Triangle()
		{
			memset(this, 0xff, sizeof(Triangle));
		}

		size_t m_positions[3];
		size_t m_normals[3];
		size_t m_textureCoordinates[3];
	};

	std::vector<Node> m_nodes;
	std::vector<Triangle> m_triangles;
	std::vector<Vec3f> m_positions;
	std::vector<Vec3f> m_normals;
	std::vector<Vec3f> m_textureCoordinates;
	Aabb m_aabb;

	void buildRecurse(struct BuildLeaf& leaf, Node& node, int depth);
	bool intersectRecurse(const Ray& ray, size_t node, float tMin, float tMax, float& hitT) const;
};