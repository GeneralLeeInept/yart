#include "mesh.h"

#include "raydifferentials.h"
#include "renderer.h"
#include "texturecache.h"
#include <cinttypes>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <fstream>
#include <regex>
#include <sstream>

static void tokenize(const std::string& line, const char* control, std::vector<std::string>& tokens)
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

std::string getDirectory(const std::string& filename)
{
	size_t dirpart = filename.find_last_of("/", std::string::npos);
	if (dirpart != std::string::npos)
		return filename.substr(0, dirpart + 1);
	return std::string();
}

Mesh::Mesh()
{
	m_defaultMaterial.Kd = Vec3f(0.5880f, 0.5880f, 0.5880f);
}

bool Mesh::loadMtlLib(const std::string& filename, TextureCache& textureCache)
{
	std::ifstream objFile(filename);
	std::string path = getDirectory(filename);

	Material* mtl = nullptr;
	std::string line;
	while (std::getline(objFile, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "newmtl")
		{
			mtl = new Material();
			m_materialLibrary[tokens[1]] = mtl;
		}
		else if (tokens[0] == "Ns")
		{
			mtl->Ns = static_cast<float>(atof(tokens[1].c_str()));
		}
		else if (tokens[0] == "Ka")
		{
			for (int i = 0; i < 3; ++i)
			{
				mtl->Ka[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}
		}
		else if (tokens[0] == "Kd")
		{
			for (int i = 0; i < 3; ++i)
			{
				mtl->Kd[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}
		}
		else if (tokens[0] == "Ks")
		{
			for (int i = 0; i < 3; ++i)
			{
				mtl->Ks[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}
		}
		else if (tokens[0] == "map_Kd")
		{
			mtl->map_Kd = textureCache.get(path + tokens[1]);
		}
	}

	return true;
}

bool Mesh::loadObj(const std::string& filename, TextureCache& textureCache)
{
	std::ifstream objFile(filename);
	std::string path = getDirectory(filename);
	Material* useMaterial = &m_defaultMaterial;

	std::string line;
	while (std::getline(objFile, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "mtllib")
		{
			loadMtlLib(path + tokens[1], textureCache);
		}
		else if (tokens[0] == "usemtl")
		{
			auto it = m_materialLibrary.find(tokens[1]);
			if (it != m_materialLibrary.end())
				useMaterial = it->second;
			else
				useMaterial = &m_defaultMaterial;
		}
		else if (tokens[0] == "v")
		{
			Vec3f v;
			for (size_t i = 0; i < 3; ++i)
			{
				v[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}
			m_positionData.push_back(v);
		}
		else if (tokens[0] == "vt")
		{
			Vec3f v;
			for (size_t i = 0; i < 3; ++i)
			{
				v[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}
			m_texcoordData.push_back(v);
		}
		else if (tokens[0] == "vn")
		{
			Vec3f v;
			for (size_t i = 0; i < 3; ++i)
			{
				v[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
			}
			m_normalData.push_back(v);
		}
		else if (tokens[0] == "f")
		{
			std::vector<int> positions;
			std::vector<int> normals;
			std::vector<int> texcoords;

			for (size_t i = 1; i < tokens.size(); ++i)
			{
				int fileIndex = atoi(tokens[i].c_str());

				if (fileIndex < 0)
				{
					positions.push_back(((int)m_positionData.size()) + fileIndex);
				}
				else if (fileIndex > 0)
				{
					positions.push_back(fileIndex - 1);
				}
				else
				{
					positions.push_back(0);
				}

				size_t vt = tokens[i].find_first_of("/", 0);
				size_t vn = std::string::npos;

				if (vt != std::string::npos && vt < tokens[i].size() - 1 && tokens[i].at(vt + 1) != '/')
				{
					int fileIndex = atoi(tokens[i].substr(vt + 1).c_str());

					if (fileIndex < 0)
					{
						texcoords.push_back(((int)m_texcoordData.size()) + fileIndex);
					}
					else if (fileIndex > 0)
					{
						texcoords.push_back(fileIndex - 1);
					}
					else
					{
						texcoords.push_back(0);
					}

					vn = tokens[i].find_first_of("/,", vt + 1);
				}

				if (vn != std::string::npos && vn < tokens[i].size() - 1)
				{
					int fileIndex = atoi(tokens[i].substr(vn + 1).c_str());

					if (fileIndex < 0)
					{
						normals.push_back(((int)m_normalData.size()) + fileIndex);
					}
					else if (fileIndex > 0)
					{
						normals.push_back(fileIndex - 1);
					}
					else
					{
						normals.push_back(0);
					}
				}
			}

			for (size_t i = 2; i < positions.size(); ++i)
			{
				Triangle t;
				t.v1 = positions[0];
				t.v2 = positions[i - 1];
				t.v3 = positions[i];
				m_positions.push_back(t);
				m_materials.push_back(useMaterial);
			}

			for (size_t i = 2; i < texcoords.size(); ++i)
			{
				Triangle t;
				t.v1 = texcoords[0];
				t.v2 = texcoords[i - 1];
				t.v3 = texcoords[i];
				m_texcoords.push_back(t);
			}

			for (size_t i = 2; i < normals.size(); ++i)
			{
				Triangle t;
				t.v1 = normals[0];
				t.v2 = normals[i - 1];
				t.v3 = normals[i];
				m_normals.push_back(t);
			}
		}
	}

	computeNormals();

	return true;
}

enum PlyElement
{
	Vertex,
	Face,
	Custom
};

bool Mesh::loadPly(const std::string& filename)
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
					Vec3f v;
					for (size_t j = 0; j < 3; ++j)
					{
						v[j] = static_cast<float>(atof(tokens[j].c_str()));
					}
					m_positionData.push_back(v);
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
						t.v1 = indices[0];
						t.v2 = indices[j - 1];
						t.v3 = indices[j];
						m_positions.push_back(t);
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

	computeNormals();

	return true;
}

Vec3f calculateNormal(const Vec3f& V1, const Vec3f& V2, const Vec3f& V3)
{
	Vec3f V1V2 = V2 - V1;
	Vec3f V1V3 = V3 - V1;
	Vec3f N = Vec3f::cross(V1V2, V1V3);
	N.normalise();
	return N;
}

void Mesh::computeNormals()
{
	if (m_normals.size() == m_positions.size())
		return;

	std::vector<Vec3f> faceNormals;
	for (const auto& tri : m_positions)
	{
		Vec3f N = calculateNormal(m_positionData[tri.v1], m_positionData[tri.v2], m_positionData[tri.v3]);
		faceNormals.push_back(N);
	}

	m_normalData.resize(m_positionData.size());

	for (size_t i = 0; i < m_positions.size(); ++i)
	{
		m_normalData[m_positions[i].v1] = m_normalData[m_positions[i].v1] + faceNormals[i];
		m_normalData[m_positions[i].v2] = m_normalData[m_positions[i].v2] + faceNormals[i];
		m_normalData[m_positions[i].v3] = m_normalData[m_positions[i].v3] + faceNormals[i];
	}

	for (auto& N : m_normalData)
	{
		N.normalise();
	}

	m_normals = m_positions;
}

void Mesh::shade(const Vec3f& P, const Vec3f& N, const RTCRay& ray, Vec3f& colour, RayDifferentials& rd) const
{
	Triangle tri = m_normals[ray.primID];
	Vec3f CN = m_normalData[tri.v1];
	CN.scale(1 - ray.u - ray.v);
	CN.scaleAdd(m_normalData[tri.v2], ray.u);
	CN.scaleAdd(m_normalData[tri.v3], ray.v);

	Vec3f ST(ray.u, ray.v, 0.0f);

	if (m_texcoords.size() > 0)
	{
		tri = m_texcoords[ray.primID];
		ST = m_texcoordData[tri.v1];
		ST.scale(1 - ray.u - ray.v);
		ST.scaleAdd(m_texcoordData[tri.v2], ray.u);
		ST.scaleAdd(m_texcoordData[tri.v3], ray.v);
	}

	Vec3f V(ray.dir);

	rd.transfer(V, CN, ray.tfar);

	m_materials[ray.primID]->shade(-V, P, CN, ST, ray.u, ray.v, colour, rd);
}
