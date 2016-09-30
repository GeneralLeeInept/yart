#pragma once

#include "material.h"
#include "sceneobject.h"
#include "vec3f.h"
#include <memory>
#include <string>
#include <vector>

class Mesh : public SceneObject
{
public:
	Mesh(const Renderer& renderer);

	bool loadObj(const char* filename);
	bool loadPly(const char* filename);

	void computeNormals();

	void shade(const Vec3f& P, const Vec3f& N, unsigned primId, float u, float v, Vec3f& colour) const;

	struct Triangle
	{
		int v1, v2, v3;
	};

	std::vector<Vec3f> m_positionData;
	std::vector<Vec3f> m_normalData;
	std::vector<Triangle> m_positions;
	std::vector<Triangle> m_normals;
	Material m_default;
};
