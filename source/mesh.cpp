#include "mesh.h"

#include <cinttypes>
#include <embree2/rtcore.h>
#include <fstream>
#include <regex>
#include <sstream>

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

	while (std::getline(objFile, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "v")
		{
			Vertex v;
			for (size_t i = 0; i < 3; ++i)
			{
				v.position[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}
			m_vertices.push_back(v);
		}
		else if (tokens[0] == "f")
		{
			std::vector<int> indices;

			for (size_t i = 1; i < tokens.size(); ++i)
			{
				int fileIndex = atoi(tokens[i].c_str());
				if (fileIndex < 0)
				{
					indices.push_back(((int)m_vertices.size()) + fileIndex);
				}
				else if (fileIndex > 0)
				{
					indices.push_back(fileIndex - 1);
				}
				else
				{
					indices.push_back(0);
				}
			}

			for (size_t i = 2; i < indices.size(); ++i)
			{
				Triangle t;
				t.positions[0] = indices[0];
				t.positions[1] = indices[i - 1];
				t.positions[2] = indices[i];
				m_triangles.push_back(t);
			}
		}
	}

	return true;
}

enum PlyElement
{
	Vertex,
	Face,
	Custom
};

bool Mesh::loadPly(const char* filename)
{
	std::ifstream plyFile(filename);
	std::string line;

	if (!std::getline(plyFile, line) || line != "ply")
	{
		return false;
	}

	std::vector<std::pair<PlyElement, unsigned>> elements;

	while (std::getline(plyFile, line) && line != "end_header")
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		else if (tokens[0] == "format")
		{
			if (tokens[1] != "ascii" || tokens[2] != "1.0")
				return false;
		}
		else if (tokens[0] == "element")
		{
			int count = atoi(tokens[2].c_str());

			if (tokens[1] == "vertex")
			{
				elements.push_back(std::pair<PlyElement, unsigned>(PlyElement::Vertex, count));
			}
			else if (tokens[1] == "face")
			{
				elements.push_back(std::pair<PlyElement, unsigned>(PlyElement::Face, count));
			}
			else
			{
				elements.push_back(std::pair<PlyElement, unsigned>(PlyElement::Custom, count));
			}
		}
	}

	for (const auto& element : elements)
	{
		switch (element.first)
		{
			case PlyElement::Vertex:
			{
				for (unsigned i = 0; i < element.second; ++i)
				{
					std::getline(plyFile, line);
					std::vector<std::string> tokens;
					tokenize(line, " \t\r\n", tokens);
					Vertex v;
					for (size_t j = 0; j < 3; ++j)
					{
						v.position[j] = static_cast<float>(atof(tokens[j].c_str()));
					}
					m_vertices.push_back(v);
				}
				break;
			}
			case PlyElement::Face:
			{
				for (unsigned i = 0; i < element.second; ++i)
				{
					std::getline(plyFile, line);
					std::vector<std::string> tokens;
					tokenize(line, " \t\r\n", tokens);
					std::vector<int> indices;

					int listLength = atoi(tokens[0].c_str());

					for (int j = 1; j <= listLength; ++j)
					{
						indices.push_back(atoi(tokens[j].c_str()));
					}

					for (size_t j = 2; j < indices.size(); ++j)
					{
						Triangle t;
						t.positions[0] = indices[0];
						t.positions[1] = indices[j - 1];
						t.positions[2] = indices[j];
						m_triangles.push_back(t);
					}
				}

				break;
			}
			case PlyElement::Custom:
			{
				for (unsigned i = 0; i < element.second; ++i)
				{
					std::getline(plyFile, line);
				}
			}
		}
	}

	return true;
}

void Mesh::addToScene(__RTCScene* scene)
{
	if (m_triangles.size() > 0 && m_vertices.size() > 0)
	{
		unsigned geomId = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, m_triangles.size(), m_vertices.size());

		float* geomVertices = (float*)rtcMapBuffer(scene, geomId, RTC_VERTEX_BUFFER);
		memcpy(geomVertices, &m_vertices[0], sizeof(Vertex) * m_vertices.size());
		rtcUnmapBuffer(scene, geomId, RTC_VERTEX_BUFFER);

		int* geomTriangles = (int*)rtcMapBuffer(scene, geomId, RTC_INDEX_BUFFER);
		memcpy(geomTriangles, &m_triangles[0], sizeof(Triangle) * m_triangles.size());
		rtcUnmapBuffer(scene, geomId, RTC_INDEX_BUFFER);
	}
}
