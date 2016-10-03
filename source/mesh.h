#pragma once

#include "material.h"
#include "sceneobject.h"
#include "vec3f.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class TextureCache;

class Mesh : public SceneObject
{
public:
	Mesh();

	bool loadMtlLib(const std::string& filename, TextureCache& textureCache);
	bool loadObj(const std::string& filename, TextureCache& textureCache);
	bool loadPly(const std::string& filename);

	void computeNormals();

	void shade(const Vec3f& P, const Vec3f& N, const RTCRay& ray, Vec3f& colour, RayDifferentials& rd) const;

	struct Triangle
	{
		int v1, v2, v3;
	};

	std::vector<Vec3f> m_positionData;
	std::vector<Vec3f> m_texcoordData;
	std::vector<Vec3f> m_normalData;
	std::vector<Triangle> m_positions;
	std::vector<Triangle> m_texcoords;
	std::vector<Triangle> m_normals;
	std::vector<Material*> m_materials;
	std::unordered_map<std::string, Material*> m_materialLibrary;

	Material m_defaultMaterial;
};
