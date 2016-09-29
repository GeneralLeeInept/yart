#include "camera.h"
#include "colour.h"
#include "math.h"
#include "mesh.h"
#include "renderer.h"
#include "rendertarget.h"
#include <FreeImage.h>

int main(int argc, char* argv)
{
	FreeImage_Initialise();

	Renderer renderer;

	Mesh* mesh = new Mesh(renderer);
	//	mesh->loadObj("C:/Development/raytracer/data/crytek-sponza/sponza.obj");
	//	mesh->loadObj("C:/Development/raytracer/data/teapot/teapot.obj");
	//	mesh->loadObj("C:/Development/raytracer/data/buddha/buddha.obj");
	//	mesh->loadObj("C:/Development/raytracer/data/dragon/dragon.obj");
	//	mesh->loadPly("C:/Development/raytracer/data/armadillo.ply");
	//	mesh->loadPly("C:/Development/raytracer/data/dragon.ply");
	//	mesh->loadPly("C:/Development/raytracer/data/happy.ply");
	//	mesh->loadObj("C:/Devel/derek.burnheim/raytracer/data/crytek-sponza/sponza.obj");
	mesh->loadObj("C:/Devel/derek.burnheim/raytracer/data/teapot/teapot.obj");
	//	mesh->loadObj("C:/Devel/derek.burnheim/raytracer/data/buddha/buddha.obj");
	//	mesh->loadObj("C:/Devel/derek.burnheim/raytracer/data/dragon/dragon.obj");
	//	mesh->loadObj("C:/Devel/derek.burnheim/raytracer/data/crytek-sponza/sponza.obj");
	//	mesh->loadPly("C:/Users/derek.burnheim/Documents/3dmodels/dragon.ply");
	//	mesh->loadPly("C:/Users/derek.burnheim/Documents/3dmodels/happy.ply");
	//	mesh->loadPly("C:/Users/derek.burnheim/Documents/3dmodels/bunny.ply");
	mesh->computeNormals();

	renderer.addMesh(mesh);

	Camera camera;
	camera.m_orientation.rotateY(static_cast<float>(M_PI));
	camera.m_position = camera.m_orientation * Vec3f(0.0f, 25.0f, -100.0f);

	RenderTarget target(600, 600);
	renderer.render(camera, target);
	target.save("test.png");

	FreeImage_DeInitialise();

	return 0;
}