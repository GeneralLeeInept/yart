#include "texturecache.h"

#include "filepath.h"
#include "texture.h"
#include <FreeImage.h>
#include <algorithm>

Texture* TextureCache::get(const std::string& filename)
{
	FilePath path(filename);

	auto it = m_textures.find(path);
	if (it != m_textures.end())
		return it->second.get();

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path);
	if (format == FIF_UNKNOWN)
		return nullptr;

	FIBITMAP* bitmap = FreeImage_Load(format, path);
	if (bitmap)
	{
		m_textures[path] = std::make_unique<Texture>(bitmap);
	}

	return m_textures[path].get();
}

std::string TextureCache::makeCanonical(const std::string& path)
{
	std::string canonical = path;
	std::replace(canonical.begin(), canonical.end(), '\\', '/');
	return canonical;
}
