#pragma once

#include "texture.h"
#include <memory>
#include <string>
#include <unordered_map>

class TextureCache
{
public:
	Texture* get(const std::string& filename);

private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;

	std::string makeCanonical(const std::string& path);
};