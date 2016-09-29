#pragma once

#include "vec3f.h"
#include <memory>
#include <string>
#include <vector>

class Mesh
{
public:
	bool loadObj(const char* filename);
	bool loadPly(const char* filename);

	Vec3f shade(const Vec3f& P, const Vec3f& N, float u, float v) const;

	struct Triangle
	{
		int v1, v2, v3;
	};

	std::vector<Vec3f> m_positionData;
	std::vector<Vec3f> m_normalData;
	std::vector<Triangle> m_positions;
	std::vector<Triangle> m_normals;
};
