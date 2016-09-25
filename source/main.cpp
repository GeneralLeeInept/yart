#include "camera.h"
#include "colour.h"
#include "geometry.h"
#include "kdtree.h"
#include "light.h"
#include "mesh.h"
#include "ray.h"
#include "rendertarget.h"
#include "scene.h"
#include "sphere.h"
#include <FreeImage.h>
#include <chrono>
#include <iostream>

void render(const Camera& camera, const Scene& scene, RenderTarget& target)
{
	float uscale = 1.0f / static_cast<float>(target.getWidth() - 1);
	float vscale = 1.0f / static_cast<float>(target.getHeight() - 1);

	std::cout.precision(0);

	std::cout << "Rendering.. 0%" << std::flush;

	float progress = 0.0f;
	float progressPerLine = 100.0f / target.getHeight();

	std::chrono::system_clock::time_point start, end;
	start = std::chrono::system_clock::now();

	for (unsigned y = 0; y < target.getHeight(); ++y)
	{
		float fy = static_cast<float>(y) + 0.5f;

		for (unsigned x = 0; x < target.getWidth(); ++x)
		{
			float fx = static_cast<float>(x) + 0.5f;
			float u = 2.0f * (fx * uscale) - 1.0f;
			float v = 1.0f - 2.0f * (fy * vscale);
			Ray ray = camera.createRay(u, v);
			Colour c;
			scene.castRay(ray, c);
			target.setPixel(x, y, c);
		}

		progress += progressPerLine;
		std::cout << "\rRendering.. " << std::fixed << progress << "%" << std::flush;
	}

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout.precision(1);
	std::cout << "\rRendering completed in " << duration.count() << "s" << std::endl;
}

int main(int argc, char* argv)
{
	geometry::tests();

	FreeImage_Initialise();

	{
		RenderTarget target(600, 600);
		Camera camera;
		Scene scene;
		scene.addLight(Light(Vec3f(4.0f, -4.0f, -1.0f), Vec3f(1.0f, 0.0f, 0.0f)));
		scene.addLight(Light(Vec3f(-4.0f, -4.0f, -1.0f), Vec3f(0.0, 1.0f, 0.0f)));
		scene.addLight(Light(Vec3f(0.0f, 4.0f, -1.0f), Vec3f(0.0f, 0.0f, 1.0f)));
		//scene.addObject(new Sphere(Vec3f(4.0, 0.0, 8.0), 2.0));
		//scene.addObject(new Sphere(Vec3f(-4.0, 0.0, 8.0), 2.0));
		//scene.addObject(new Sphere(Vec3f(0.0, 0.0, 511.0), 500.0));
		//scene.addObject(new Sphere(Vec3f(0.0, 0.0, 0.0), 1.0));
		Mesh* teapot = new Mesh();
		teapot->loadObj("teapot/teapot.obj");
		KdTree* kdtree = new KdTree();
		kdtree->build(*teapot);
		scene.addObject(kdtree);
		camera.m_position = Vec3f(0.0f, 25.0f, -50.0f);
		render(camera, scene, target);
		target.save("test.png");
	}

	FreeImage_DeInitialise();

	return 0;
}