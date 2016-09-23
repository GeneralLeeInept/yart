#include "mesh.h"

#include <fstream>
#include <regex>
#include <sstream>

#include "ray.h"

struct Mesh::Material
{
	std::string name;
	std::string map_d;
	float d; /*! opacity value        */
	std::string map_Ka;
	Vec3f Ka; /*! ambient color        */
	std::string map_Kd;
	Vec3f Kd; /*! diffuse color        */
	std::string map_Ks;
	Vec3f Ks; /*! specular color       */
	std::string map_Ns;
	float Ns; /*! specular coefficient */
	std::string map_Bump; /*! bump map */

	Material(const std::string& name = "")
	  : name(name)
	{
		d = 1.0f;
		Ka.set(0.5f);
		Kd.set(0.5f);
		Ns = 0.0f;
	}
};

void tokenize(const std::string& line, const char* control, std::vector<std::string>& tokens)
{
	size_t pos = line.find_first_not_of(control, 0);

	while (pos != std::string::npos)
	{
		size_t eot = line.find_first_of(control, pos);
		size_t tokenLength = (eot == std::string::npos) ? eot : (eot - pos);
		std::string token = line.substr(pos, tokenLength);
		tokens.push_back(token);
		pos = line.find_first_not_of(control, eot);
	}
}

bool Mesh::loadObj(const char* filename)
{
	std::ifstream objFile(filename);
	std::string line;
	MicroMesh* microMesh = nullptr;
	IndexMap indexMap;

	while (std::getline(objFile, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty() || tokens[0] == "#")
			continue;

		if (tokens[0] == "v")
		{
			Vec3f P;

			for (size_t i = 0; i < 3; ++i)
			{
				P.v[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}

			m_positions.push_back(P);
		}
		else if (tokens[0] == "vn")
		{
			Vec3f N;

			for (size_t i = 0; i < 3; ++i)
			{
				N.v[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}

			m_normals.push_back(N);
		}
		else if (tokens[0] == "vt")
		{
			Vec3f V(0.0f);

			for (size_t i = 0; i < 3 && i < tokens.size() - 1; ++i)
			{
				V.v[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}

			m_textureCoordinates.push_back(V);
		}
		else if (tokens[0] == "g")
		{
			m_microMeshes.push_back(MicroMesh());
			microMesh = &m_microMeshes.back();
		}
		else if (tokens[0] == "f")
		{
			IndexArray indices;

			for (size_t i = 1; i < tokens.size(); ++i)
			{
				indices.push_back(addVertex(tokens[i], indexMap));
			}

			for (size_t i = 2; i < indices.size(); ++i)
			{
				microMesh->m_indices.push_back(indices[0]);
				microMesh->m_indices.push_back(indices[i - 1]);
				microMesh->m_indices.push_back(indices[i]);
			}
		}
	}

	return true;
}

#define EPSILON 0.000001

bool triangle_intersection(const Vec3f& V1, const Vec3f& V2, const Vec3f& V3, const Ray& ray, float& t, float& u,
                           float& v)
{
	Vec3f e1, e2; // Edge1, Edge2
	Vec3f P, Q, T;
	float det, inv_det;

	// Find vectors for two edges sharing V1
	e1 = V2 - V1;
	e2 = V3 - V1;

	// Begin calculating determinant - also used to calculate u parameter
	P = Vec3f::cross(ray.m_direction, e2);

	// if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	det = Vec3f::dot(e1, P);
	// NOT CULLING
	if (det > -EPSILON && det < EPSILON)
		return 0;

	inv_det = 1.f / det;

	// calculate distance from V1 to ray origin
	T = ray.m_origin - V1;

	// Calculate u parameter and test bound
	u = Vec3f::dot(T, P) * inv_det;
	// The intersection lies outside of the triangle
	if (u < 0.f || u > 1.f)
		return 0;

	// Prepare to test v parameter
	Q = Vec3f::cross(T, e1);

	// Calculate V parameter and test bound
	v = Vec3f::dot(ray.m_direction, Q) * inv_det;
	// The intersection lies outside of the triangle
	if (v < 0.f || u + v > 1.f)
		return 0;

	t = Vec3f::dot(e2, Q) * inv_det;

	return (t > EPSILON);
}

bool Mesh::intersect(const Ray& ray, float& t) const
{
	t = std::numeric_limits<float>::max();

	for (auto& micromesh : m_microMeshes)
	{
		int numTris = micromesh.m_indices.size() / 3;
		for (int tri = 0; tri < numTris; ++tri)
		{
			const Vertex& v1 = m_vertices[micromesh.m_indices[tri * 3 + 0]];
			const Vertex& v2 = m_vertices[micromesh.m_indices[tri * 3 + 1]];
			const Vertex& v3 = m_vertices[micromesh.m_indices[tri * 3 + 2]];
			float tHit, u, v;
			if (triangle_intersection(m_positions[v1.m_position], m_positions[v2.m_position],
			                          m_positions[v3.m_position], ray, tHit, u, v)
			    && tHit < t)
			{
				t = tHit;
			}
		}
	}

	return t < std::numeric_limits<float>::max();
}

void Mesh::getSurfaceData(HitData& hitData) const
{
}

int fixIndex(int fileIndex, size_t last)
{
	if (fileIndex < 0)
	{
		fileIndex = last - fileIndex + 1;
	}
	return fileIndex - 1;
}

size_t Mesh::addVertex(const std::string& decl, IndexMap& indexMap)
{
	std::regex re("(\\d+)(/(\\d+)?)?(/(\\d+)?)?");
	std::smatch matches;
	Vertex v = { invalid, invalid, invalid };

	if (std::regex_match(decl, matches, re))
	{
		if (matches[1].matched)
		{
			v.m_position = fixIndex(atoi(matches[1].str().c_str()), m_positions.size());
		}
		if (matches[3].matched)
		{
			v.m_textureCoordinates = fixIndex(atoi(matches[3].str().c_str()), m_textureCoordinates.size());
		}
		if (matches[5].matched)
		{
			v.m_normal = fixIndex(atoi(matches[5].str().c_str()), m_normals.size());
		}
	}

	auto it = indexMap.find(v);
	size_t index;

	if (it == indexMap.end())
	{
		index = m_vertices.size();
		m_vertices.push_back(v);
		indexMap[v] = index;
	}
	else
	{
		index = it->second;
	}

	return index;
}

size_t Mesh::VertexHasher::operator()(const Vertex& v) const
{
	return std::hash<size_t>()(v.m_position);
}

bool Mesh::VertexComparer::operator()(const Vertex& v1, const Vertex& v2) const
{
	return (v1.m_position == v2.m_position && v1.m_normal == v2.m_normal
	        && v1.m_textureCoordinates == v2.m_textureCoordinates);
}
