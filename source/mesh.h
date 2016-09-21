#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "colour.h"
#include "renderable.h"
#include "vec2.h"
#include "vec3f.h"

class Mesh : public IRenderable
{
public:
	bool loadObj(const char* filename);

	bool intersect(const Ray& ray, float& t) const;
	void getSurfaceData(const Vec3f& Phit, Vec3f& Nhit, Vec2& tex) const;

private:
	struct Material
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
			Ka.m_x = 0.5f, Ka.m_y = 0.5f, Ka.m_z = 0.5f;
			d = 1.0f;
			Kd.m_x = 0.5f, Kd.m_y = 0.5f, Kd.m_z = 0.5f;
			Ns = 0.0f;
			Ks.m_x = 0.0f, Ks.m_y = 0.0f, Ks.m_z = 0.0f;
		}
	};

	typedef std::shared_ptr<Material> MaterialPtr;

	struct Vertex
	{
		Vec3f m_position;
		Vec3f m_normal;
		Vec2 m_textureCoordinate;
	};

	typedef std::vector<Vertex> VertexArray;
	typedef VertexArray::size_type Index;
	typedef std::vector<Index> IndexArray;

	struct MicroMesh
	{
		MaterialPtr m_material;
		IndexArray m_indices;
	};

	Index addVertex(const std::string& decl, const std::vector<Vec3f>& vertices, const std::vector<Vec3f>& normals,
	                const std::vector<Vec2>& textureCoords);

	std::map<std::string, MaterialPtr> m_materials;
	VertexArray m_vertices;
	std::vector<MicroMesh> m_microMeshes;
};
