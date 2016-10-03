#include "camera.h"
#include "colour.h"
#include "filepath.h"
#include "math.h"
#include "mesh.h"
#include "meshcache.h"
#include "renderer.h"
#include "rendertarget.h"
#include "scene.h"
#include "texturecache.h"
#include <FreeImage.h>

int main(int argc, char** argv)
{
	FreeImage_Initialise();

	FilePath path(argv[1]);
	Renderer renderer;
	TextureCache textureCache;
	MeshCache meshCache;
	std::unique_ptr<Scene> scene = std::unique_ptr<Scene>(Scene::load(path, meshCache, textureCache));
	renderer.setScene(scene.get());
	RenderTarget target(640 * 4, 480 * 4);
	renderer.render(target);

	path.changeExtension(".png");
	target.save(path);

	FreeImage_DeInitialise();

	return 0;
}