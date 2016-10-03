#include "camera.h"
#include "colour.h"
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

	Renderer renderer;
	TextureCache textureCache;
	MeshCache meshCache;
	std::unique_ptr<Scene> scene = std::unique_ptr<Scene>(Scene::load(argv[1], meshCache, textureCache));
	renderer.setScene(scene.get());
	RenderTarget target(640, 480);
	renderer.render(target);
	target.save("test.png");

	FreeImage_DeInitialise();

	return 0;
}