#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Mesh;
class TextureCache;

class MeshCache
{
public:
	Mesh* get(const std::string& filename, TextureCache& textureCache);

private:
	std::unordered_map<std::string, std::unique_ptr<Mesh>> m_cache;
};