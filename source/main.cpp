#include "camera.h"
#include "colour.h"
#include "math.h"
#include "mesh.h"
#include "rendertarget.h"
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <FreeImage.h>
#include <chrono>
#include <iostream>
#include <memory>

void render(const Camera& camera, RTCScene scene, RenderTarget& target)
{
	float uscale = 1.0f / static_cast<float>(target.getWidth() - 1);
	float vscale = 1.0f / static_cast<float>(target.getHeight() - 1);

	std::cout.precision(0);

	std::cout << "Rendering.. 0%" << std::flush;

	float progress = 0.0f;
	float progressPerLine = 100.0f / target.getHeight();

	std::chrono::system_clock::time_point start, end;
	start = std::chrono::system_clock::now();

	Vec3f lightP(1.f, 1.f, 1.f);	// light position

	for (unsigned y = 0; y < target.getHeight(); ++y)
	{
		float fy = static_cast<float>(y) + 0.5f;

		for (unsigned x = 0; x < target.getWidth(); ++x)
		{
			float fx = static_cast<float>(x) + 0.5f;
			float u = 2.0f * (fx * uscale) - 1.0f;
			float v = 1.0f - 2.0f * (fy * vscale);
			RTCRay ray;
			camera.createRay(ray, u, v);
			rtcIntersect(scene, ray);
			if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
			{
				Vec3f N(ray.Ng);
				N.normalise();
				Vec3f hitP(ray.org);
				hitP.scaleAdd(Vec3f(ray.dir), ray.tfar);
				Vec3f L = lightP - hitP;
				L.normalise();
				float nDotL = -Vec3f::dot(N, L);
				uint8_t l = 0;
				if (compareFloats(nDotL, 0.0f) > 0)
					l = static_cast<uint8_t>(nDotL * 255.0f);
				target.setPixel(x, y, Colour(l, l, l));
			}
			else
				target.setPixel(x, y, Colour(0, 0, 0));
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
	FreeImage_Initialise();
	auto deviceDeleter = [&](__RTCDevice* ptr) { rtcDeleteDevice(ptr); };
	std::unique_ptr<__RTCDevice, decltype(deviceDeleter)> device(rtcNewDevice(), deviceDeleter);
	auto sceneDeleter = [&](__RTCScene* ptr) { rtcDeleteScene(ptr); };
	std::unique_ptr<__RTCScene, decltype(sceneDeleter)> scene(
	  rtcDeviceNewScene(device.get(), RTC_SCENE_STATIC, RTC_INTERSECT1), sceneDeleter);
	Mesh mesh;
//	mesh.loadObj("C:/Development/raytracer/data/crytek-sponza/sponza.obj");
//	mesh.loadObj("C:/Development/raytracer/data/teapot/teapot.obj");
	mesh.loadObj("C:/Development/raytracer/data/buddha/buddha.obj");
//	mesh.loadObj("C:/Development/raytracer/data/dragon/dragon.obj");
//	mesh.loadPly("C:/Development/raytracer/data/armadillo.ply");
//	mesh.loadPly("C:/Development/raytracer/data/dragon.ply");
//	mesh.loadPly("C:/Development/raytracer/data/happy.ply");
//	mesh.loadPly("C:/Users/derek.burnheim/Documents/3dmodels/dragon.ply");
//	mesh.loadPly("C:/Users/derek.burnheim/Documents/3dmodels/happy.ply");
//	mesh.loadPly("C:/Users/derek.burnheim/Documents/3dmodels/bunny.ply");
	mesh.addToScene(scene.get());
	rtcCommit(scene.get());

	std::unique_ptr<RenderTarget> renderTarget(new RenderTarget(600, 600));
	Camera camera;
	camera.m_orientation.rotateY(static_cast<float>(-M_PI_2));
	camera.m_position = Vec3f(1.f, 0.0f, 0.0f);
	render(camera, scene.get(), *(renderTarget.get()));
	renderTarget->save("test.png");

	FreeImage_DeInitialise();

	return 0;
}