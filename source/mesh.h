#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "colour.h"
#include "vec2.h"
#include "vec3.h"

class Mesh
{
public:
	bool loadObj(const char* filename);

private:
	struct Material
	{
		std::string name;
		std::string map_d;
		double d; /*! opacity value        */
		std::string map_Ka;
		Colour Ka; /*! ambient color        */
		std::string map_Kd;
		Colour Kd; /*! diffuse color        */
		std::string map_Ks;
		Colour Ks; /*! specular color       */
		std::string map_Ns;
		double Ns; /*! specular coefficient */
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
		Vec3 m_position;
		Vec3 m_normal;
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

	Index addVertex(const std::string& decl, const std::vector<Vec3>& vertices, const std::vector<Vec3>& normals,
	                const std::vector<Vec2>& textureCoords);

	std::map<std::string, MaterialPtr> m_materials;
	VertexArray m_vertices;
	std::vector<MicroMesh> m_microMeshes;
};
