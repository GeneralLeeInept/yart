#include <fstream>
#include <regex>
#include <sstream>

#include "mesh.h"

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
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> textureCoordinates;
	MicroMesh* microMesh = nullptr;

	while (std::getline(objFile, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty() || tokens[0] == "#")
			continue;

		if (tokens[0] == "v")
		{
			Vec3 v;

			for (int i = 0; i < 3; ++i)
			{
				v.m_v[i] = atof(tokens[i + 1].c_str());
			}

			vertices.push_back(v);
		}
		else if (tokens[0] == "vn")
		{
			Vec3 v;

			for (int i = 0; i < 3; ++i)
			{
				v.m_v[i] = atof(tokens[i + 1].c_str());
			}

			normals.push_back(v);
		}
		else if (tokens[0] == "vt")
		{
			Vec2 v;

			for (int i = 0; i < 2; ++i)
			{
				v.m_v[i] = atof(tokens[i + 1].c_str());
			}

			textureCoordinates.push_back(v);
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
				indices.push_back(addVertex(tokens[i], vertices, normals, textureCoordinates));
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

int fixIndex(int fileIndex, size_t last)
{
	if (fileIndex < 0)
	{
		fileIndex = last - fileIndex + 1;
	}
	return fileIndex - 1;
}

Mesh::Index Mesh::addVertex(const std::string& decl, const std::vector<Vec3>& vertices,
                            const std::vector<Vec3>& normals, const std::vector<Vec2>& textureCoords)
{
	int indices[3] = {0};
	std::regex re("(\\d+)(/(\\d+)?)?(/(\\d+)?)?");
	std::smatch matches;
	if (std::regex_match(decl, matches, re))
	{
		for (int i = 0; i < 3; ++i)
		{
			if (matches[i * 2 + 1].matched)
				indices[i] = atoi(matches[(i * 2) + 1].str().c_str());
		}
	}

	Vertex v;
	if (indices[0])
	{
		v.m_position = vertices[fixIndex(indices[0], vertices.size())];
	}
	if (indices[2])
	{
		v.m_normal = normals[fixIndex(indices[2], normals.size())];
	}
	if (indices[1])
	{
		v.m_textureCoordinate = textureCoords[fixIndex(indices[1], textureCoords.size())];
	}

	m_vertices.push_back(v);

	return m_vertices.size() - 1;
}
