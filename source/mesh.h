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

	void addToScene(struct __RTCScene* scene);

	struct Vertex
	{
		Vec3f position;
		float pad;
	};

	struct Triangle
	{
		int positions[3];
	};

	std::vector<Vertex> m_vertices;
	std::vector<Triangle> m_triangles;
};
