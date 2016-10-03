#include "meshcache.h"

#include "filepath.h"
#include "mesh.h"
#include <algorithm>

Mesh* MeshCache::get(const std::string& filename, TextureCache& textureCache)
{
	FilePath path(filename);

	auto it = m_cache.find(path);
	if (it != m_cache.end())
		return it->second.get();

	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
	std::string extension = path.extension();
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	if (extension == ".obj")
	{
		if (mesh->loadObj(path, textureCache))
		{
			m_cache[path] = std::move(mesh);
		}
	}
	else if (extension == ".ply")
	{
		if (mesh->loadPly(path))
		{
			m_cache[path] = std::move(mesh);
		}
	}
	return m_cache[path].get();
}
