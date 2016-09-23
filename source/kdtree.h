#pragma once

#include <vector>

class Mesh;
class Vec3f;

class KdTree
{
public:
	void Build(const Mesh& mesh);

private:
	union Node;
	struct Triangle;
	struct Material;

	std::vector<Node> m_nodes;
	std::vector<Triangle> m_triangles;
	std::vector<Vec3f> m_vertices;
	std::vector<Vec3f> m_normals;
	std::vector<Vec3f> m_textureCoordinates;
	std::vector<Material> m_materials;
};