#include "texturemanager.h"

#include "texture.h"
#include <FreeImage.h>

TexturePtr TextureManager::load(const std::string& filename)
{
	std::string canonicalized;
	canonicalizeFilename(filename, canonicalized);

	auto it = m_textures.find(filename);
	if (it != m_textures.end())
		return it->second;

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(canonicalized.c_str());
	if (format == FIF_UNKNOWN)
		return nullptr;

	FIBITMAP* bitmap = FreeImage_Load(format, canonicalized.c_str());
	if (bitmap)
	{
		m_textures[canonicalized] = TexturePtr(new Texture(bitmap));
	}

	return m_textures[canonicalized];
}

void TextureManager::canonicalizeFilename(const std::string& filename, std::string& canonicalized)
{
	canonicalized = filename;
}
