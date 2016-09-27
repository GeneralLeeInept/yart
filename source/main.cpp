#include "camera.h"
#include "colour.h"
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
				target.setPixel(x, y, Colour(255, 255, 255));
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
	mesh.load("dragon/dragon.obj", scene.get());
	rtcCommit(scene.get());

	std::unique_ptr<RenderTarget> renderTarget(new RenderTarget(600, 600));
	Camera camera;
	camera.m_position = Vec3f(0.0f, 2.0f, -12.0f);
	render(camera, scene.get(), *(renderTarget.get()));
	renderTarget->save("test.png");

	FreeImage_DeInitialise();

	return 0;
}