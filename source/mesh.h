#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "colour.h"
#include "renderable.h"
#include "vec2f.h"
#include "vec3f.h"

class Mesh : public IRenderable
{
public:
	typedef std::vector<Vec3f> VectorArray;
	typedef std::vector<size_t> IndexArray;

	struct Material;
	typedef std::shared_ptr<Material> MaterialPtr;

	struct MicroMesh
	{
		MaterialPtr m_material;
		IndexArray m_indices;
	};

	bool loadObj(const char* filename);

	bool intersect(const Ray& ray, float& t) const;
	void getSurfaceData(HitData& hitData) const;
	
	struct Vertex
	{
		size_t m_position;
		size_t m_normal;
		size_t m_textureCoordinates;
	};

	typedef std::vector<Vertex> VertexArray;

	struct VertexHasher
	{
		size_t operator()(const Vertex& v) const;
	};

	struct VertexComparer
	{
		bool operator()(const Vertex& v1, const Vertex& v2) const;
	};

	typedef std::unordered_map<Vertex, size_t, VertexHasher, VertexComparer> IndexMap;
	static const size_t invalid = (size_t)-1;

	size_t addVertex(const std::string& decl, IndexMap& indexMap);

	std::unordered_map<std::string, MaterialPtr> m_materials;
	VectorArray m_positions;
	VectorArray m_normals;
	VectorArray m_textureCoordinates;
	VertexArray m_vertices;
	std::vector<MicroMesh> m_microMeshes;
};
