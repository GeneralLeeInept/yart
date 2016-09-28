#include "mesh.h"

#include <embree2/rtcore.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <cinttypes>

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

bool Mesh::loadObj(const char* filename, __RTCScene* scene)
{
	std::ifstream objFile(filename);
	std::string line;

	std::vector<float> vertices;
	std::vector<int> triangles;

	while (std::getline(objFile, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "v")
		{
			for (size_t i = 0; i < 3; ++i)
			{
				vertices.push_back(static_cast<float>(atof(tokens[i + 1].c_str())));
			}
			vertices.push_back(1.0f);
		}
		else if (tokens[0] == "f")
		{
			std::vector<int> indices;

			for (size_t i = 1; i < tokens.size(); ++i)
			{
				int fileIndex = atoi(tokens[i].c_str());
				if (fileIndex < 0)
				{
					indices.push_back(((int)vertices.size()) + fileIndex);
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
				triangles.push_back(indices[0]);
				triangles.push_back(indices[i - 1]);
				triangles.push_back(indices[i]);
			}
		}
	}

	size_t numTriangles = triangles.size() / 3;
	size_t numVertices = vertices.size() / 4;

	if (numTriangles > 0 && numVertices > 0)
	{
		unsigned geomId = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, triangles.size() / 3, vertices.size() / 4);

		float* geomVertices = (float*)rtcMapBuffer(scene, geomId, RTC_VERTEX_BUFFER);
		memcpy(geomVertices, &vertices[0], sizeof(float) * vertices.size());
		rtcUnmapBuffer(scene, geomId, RTC_VERTEX_BUFFER);

		int* geomTriangles = (int*)rtcMapBuffer(scene, geomId, RTC_INDEX_BUFFER);
		memcpy(geomTriangles, &triangles[0], sizeof(int) * triangles.size());
		rtcUnmapBuffer(scene, geomId, RTC_INDEX_BUFFER);
	}

	return true;
}

enum PlyElement
{
	Vertex,
	Face,
	Custom
};

bool Mesh::loadPly(const char * filename, __RTCScene * scene)
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
				elements.push_back(std::pair<PlyElement, unsigned>(Vertex, count));
			}
			else if (tokens[1] == "face")
			{
				elements.push_back(std::pair<PlyElement, unsigned>(Face, count));
			}
			else
			{
				elements.push_back(std::pair<PlyElement, unsigned>(Custom, count));
			}
		}
	}

	std::vector<float> vertices;
	std::vector<int> triangles;

	for (const auto& element : elements)
	{
		switch (element.first)
		{
		case Vertex:
		{
			for (unsigned i = 0; i < element.second; ++i)
			{
				std::getline(plyFile, line);
				std::vector<std::string> tokens;
				tokenize(line, " \t\r\n", tokens);
				for (size_t j = 0; j < 3; ++j)
				{
					vertices.push_back(static_cast<float>(atof(tokens[j].c_str())));
				}
				vertices.push_back(1.0f);
			}

			break;
		}
		case Face:
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
					triangles.push_back(indices[0]);
					triangles.push_back(indices[j - 1]);
					triangles.push_back(indices[j]);
				}
			}

			break;
		}
		case Custom:
		{
			for (unsigned i = 0; i < element.second; ++i)
			{
				std::getline(plyFile, line);
			}
		}
		}
	}

	size_t numTriangles = triangles.size() / 3;
	size_t numVertices = vertices.size() / 4;

	if (numTriangles > 0 && numVertices > 0)
	{
		unsigned geomId = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, triangles.size() / 3, vertices.size() / 4);

		float* geomVertices = (float*)rtcMapBuffer(scene, geomId, RTC_VERTEX_BUFFER);
		memcpy(geomVertices, &vertices[0], sizeof(float) * vertices.size());
		rtcUnmapBuffer(scene, geomId, RTC_VERTEX_BUFFER);

		int* geomTriangles = (int*)rtcMapBuffer(scene, geomId, RTC_INDEX_BUFFER);
		memcpy(geomTriangles, &triangles[0], sizeof(int) * triangles.size());
		rtcUnmapBuffer(scene, geomId, RTC_INDEX_BUFFER);
	}

	return true;
}
