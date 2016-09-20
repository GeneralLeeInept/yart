#include "camera.h"
#include "colour.h"
#include "light.h"
#include "mesh.h"
#include "ray.h"
#include "rendertarget.h"
#include "scene.h"
#include "sphere.h"
#include <FreeImage.h>

void render(const Camera& camera, const Scene& scene, RenderTarget& target)
{
	double uscale = 1.0 / static_cast<double>(target.getWidth() - 1);
	double vscale = 1.0 / static_cast<double>(target.getHeight() - 1);

	for (unsigned y = 0; y < target.getHeight(); ++y)
	{
		double fy = static_cast<double>(y) + 0.5;

		for (unsigned x = 0; x < target.getWidth(); ++x)
		{
			double fx = static_cast<double>(x) + 0.5;
			double u = 2.0 * (fx * uscale) - 1.0;
			double v = 1.0 - 2.0 * (fy * vscale);
			Ray ray = camera.createRay(u, v);
			Colour c;
			scene.castRay(ray, c);
			target.setPixel(x, y, c);
		}
	}
}

int main(int argc, char* argv)
{
	FreeImage_Initialise();

	Mesh mesh;
	mesh.loadObj("teapot/teapot.obj");

	{
		RenderTarget target(600, 600);
		Camera camera;
		Scene scene;
		scene.addLight(Light(Vec3(4.0, -4.0, 4.0), Colour(0.8, 0.0, 0.0)));
		scene.addLight(Light(Vec3(-4.0, -4.0, 4.0), Colour(0.0, 0.8, 0.0)));
		scene.addLight(Light(Vec3(0.0, 4.0, 4.0), Colour(0.0, 0.0, 0.8)));
		scene.addSphere(Sphere(Vec3(4.0, 0.0, 8.0), 2.0));
		scene.addSphere(Sphere(Vec3(-4.0, 0.0, 8.0), 2.0));
		scene.addSphere(Sphere(Vec3(0.0, 0.0, 511.0), 500.0));
		camera.m_position = Vec3(0.0, 0.0, 0.0);
		render(camera, scene, target);
		target.save("test.png");
	}

	FreeImage_DeInitialise();

	return 0;
}