#include "camera.h"
#include "colour.h"
#include "light.h"
#include "mesh.h"
#include "ray.h"
#include "rendertarget.h"
#include "scene.h"
#include "sphere.h"
#include <FreeImage.h>
#include <iostream>

void render(const Camera& camera, const Scene& scene, RenderTarget& target)
{
	double uscale = 1.0 / static_cast<double>(target.getWidth() - 1);
	double vscale = 1.0 / static_cast<double>(target.getHeight() - 1);

	std::cout.precision(0);

	std::cout << "Rendering.. 0%" << std::flush;

	double progress = 0.0;
	double progressPerLine = 100.0 / target.getHeight();

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

		progress += progressPerLine;
		std::cout << "\rRendering.. " << std::fixed << progress << "%" << std::flush;
	}

	std::cout << "\rRendering complete" << std::endl;
}

int main(int argc, char* argv)
{
	FreeImage_Initialise();

	{
		RenderTarget target(600, 600);
		Camera camera;
		Scene scene;
		scene.addLight(Light(Vec3(4.0, -4.0, -1.0), Colour(1.0, 0.0, 0.0)));
		scene.addLight(Light(Vec3(-4.0, -4.0, -1.0), Colour(0.0, 1.0, 0.0)));
		scene.addLight(Light(Vec3(0.0, 4.0, -1.0), Colour(0.0, 0.0, 1.0)));
		//scene.addObject(new Sphere(Vec3(4.0, 0.0, 8.0), 2.0));
		//scene.addObject(new Sphere(Vec3(-4.0, 0.0, 8.0), 2.0));
		//scene.addObject(new Sphere(Vec3(0.0, 0.0, 511.0), 500.0));
		//scene.addObject(new Sphere(Vec3(0.0, 0.0, 0.0), 1.0));
		Mesh* teapot = new Mesh();
		teapot->loadObj("test.obj");
		scene.addObject(teapot);
		camera.m_position = Vec3(0.0, 0.5, -5.0);
		render(camera, scene, target);
		target.save("test.png");
	}

	FreeImage_DeInitialise();

	return 0;
}