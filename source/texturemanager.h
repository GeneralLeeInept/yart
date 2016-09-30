#pragma once

#include <memory>
#include <string>
#include <unordered_map>

typedef std::shared_ptr<class Texture> TexturePtr;

class TextureManager
{
public:
	TexturePtr load(const std::string& filename);

private:
	std::unordered_map<std::string, TexturePtr> m_textures;

	void canonicalizeFilename(const std::string& filename, std::string& canonicalized);
};