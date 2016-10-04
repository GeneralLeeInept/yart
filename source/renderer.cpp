#include "renderer.h"

#include "camera.h"
#include "colour.h"
#include "mesh.h"
#include "raydifferentials.h"
#include "rendertarget.h"
#include "scene.h"
#include <chrono>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <iostream>

Renderer::Renderer()
{
	m_device = rtcNewDevice();
	m_rtcScene = nullptr;
	m_scene = nullptr;
}

Renderer::~Renderer()
{
	rtcDeleteScene(m_rtcScene);
	rtcDeleteDevice(m_device);
}

void Renderer::setScene(Scene* scene)
{
	rtcDeleteScene(m_rtcScene);
	m_scene = scene;
	if (m_scene)
	{
		m_rtcScene = rtcDeviceNewScene(m_device, RTC_SCENE_STATIC, RTC_INTERSECT1);

		for (auto& sceneNode : m_scene->m_objects)
		{
			Mesh* mesh = (Mesh*)sceneNode->object;

			if (mesh)
			{
				size_t numTriangles = mesh->m_positions.size();
				size_t numVertices = mesh->m_positionData.size();
				if (numTriangles > 0 && numVertices > 0)
				{
					unsigned geomId = rtcNewTriangleMesh(m_rtcScene, RTC_GEOMETRY_STATIC, numTriangles, numVertices);

					if (geomId != RTC_INVALID_GEOMETRY_ID)
					{
						float* geomVertices = (float*)rtcMapBuffer(m_rtcScene, geomId, RTC_VERTEX_BUFFER);
						for (const Vec3f& v : mesh->m_positionData)
						{
							for (int i = 0; i < 3; ++i)
							{
								*geomVertices++ = v[i];
							}
							++geomVertices;
						}
						rtcUnmapBuffer(m_rtcScene, geomId, RTC_VERTEX_BUFFER);

						int* geomTriangles = (int*)rtcMapBuffer(m_rtcScene, geomId, RTC_INDEX_BUFFER);
						memcpy(geomTriangles, &mesh->m_positions[0], sizeof(Mesh::Triangle) * mesh->m_positions.size());
						rtcUnmapBuffer(m_rtcScene, geomId, RTC_INDEX_BUFFER);

						m_geometry[geomId] = mesh;
					}
				}
			}
		}
	}

	rtcCommit(m_rtcScene);
}

void Renderer::render(RenderTarget& target)
{
	unsigned width = target.getWidth();
	unsigned height = target.getHeight();
	float xscale = 1.0f / static_cast<float>(width - 1);
	float yscale = 1.0f / static_cast<float>(height - 1);
	float uscale = (width > height) ? (static_cast<float>(width) / static_cast<float>(height)) : 1.0f;
	float vscale = (height > width) ? (static_cast<float>(height) / static_cast<float>(width)) : 1.0f;

	std::cout.precision(0);
	std::cout << "Rendering.. 0%" << std::flush;

	float progress = 0.0f;
	float progressPerLine = 100.0f / target.getHeight();

	std::chrono::system_clock::time_point start, end;
	start = std::chrono::system_clock::now();

	Camera camera = *(m_scene->m_camera.get());

	for (unsigned y = 0; y < target.getHeight(); ++y)
	{
		float fy = static_cast<float>(y) + 0.5f;

		for (unsigned x = 0; x < target.getWidth(); ++x)
		{
			float fx = static_cast<float>(x) + 0.5f;
			float u = 2.0f * (fx * xscale) - 1.0f;
			float v = 1.0f - 2.0f * (fy * yscale);
			RTCRay ray;
			camera.createRay(ray, u * uscale, v * vscale);
			rtcIntersect(m_rtcScene, ray);
			if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
			{
				RayDifferentials rd(camera.m_orientation.X, camera.m_orientation.Y, camera.m_orientation.Z,
				                    Vec3f(uscale, vscale, 1.0f));
				Vec3f N(ray.Ng);
				N.normalise();
				Vec3f hitP(ray.org);
				hitP.scaleAdd(Vec3f(ray.dir), ray.tfar);
				Vec3f cf;
				for (auto& light : m_scene->m_lights)
				{
					Vec3f L = light->L(hitP);

					RTCRay shadowRay;

					for (int i = 0; i < 3; ++i)
					{
						shadowRay.org[i] = hitP[i];
						shadowRay.dir[i] = L[i];
					}

					shadowRay.tnear = 0.01f; // todo: scene option, adaptive?
					shadowRay.tfar = light->distanceFrom(hitP);// std::numeric_limits<float>::infinity();
					shadowRay.instID = RTC_INVALID_GEOMETRY_ID;
					shadowRay.geomID = RTC_INVALID_GEOMETRY_ID;
					shadowRay.primID = RTC_INVALID_GEOMETRY_ID;
					shadowRay.mask = 0xffffffff;
					shadowRay.time = 0.0f;

					rtcOccluded(m_rtcScene, shadowRay);

					if (shadowRay.geomID)
						cf += m_geometry[ray.geomID]->shade(hitP, N, ray, *(light.get()), rd);
				}
				target.setPixel(x, y, Colour(cf));
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
