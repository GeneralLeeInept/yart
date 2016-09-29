#include "renderer.h"

#include "camera.h"
#include "colour.h"
#include "mesh.h"
#include "rendertarget.h"
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <chrono>
#include <iostream>

Renderer::Renderer()
{
	m_device = rtcNewDevice();
	m_scene = rtcDeviceNewScene(m_device, RTC_SCENE_STATIC, RTC_INTERSECT1);
}

Renderer::~Renderer()
{
	rtcDeleteScene(m_scene);
	rtcDeleteDevice(m_device);
}

void Renderer::addMesh(const Mesh & mesh)
{
	size_t numTriangles = mesh.m_positions.size();
	size_t numVertices = mesh.m_positionData.size();
	if (numTriangles > 0 && numVertices > 0)
	{
		unsigned geomId = rtcNewTriangleMesh(m_scene, RTC_GEOMETRY_STATIC, numTriangles, numVertices);

		if (geomId != RTC_INVALID_GEOMETRY_ID)
		{
			float* geomVertices = (float*)rtcMapBuffer(m_scene, geomId, RTC_VERTEX_BUFFER);
			for (const Vec3f& v : mesh.m_positionData)
			{
				for (int i = 0; i < 3; ++i)
				{
					*geomVertices++ = v[i];
				}
				++geomVertices;
			}
			rtcUnmapBuffer(m_scene, geomId, RTC_VERTEX_BUFFER);

			int* geomTriangles = (int*)rtcMapBuffer(m_scene, geomId, RTC_INDEX_BUFFER);
			memcpy(geomTriangles, &mesh.m_positions[0], sizeof(Mesh::Triangle) * mesh.m_positions.size());
			rtcUnmapBuffer(m_scene, geomId, RTC_INDEX_BUFFER);

			m_geometry[geomId] = &mesh;
		}
	}
}

void Renderer::commitScene()
{
	rtcCommit(m_scene);
}

void Renderer::render(const Camera & camera, RenderTarget & target)
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
			rtcIntersect(m_scene, ray);
			if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
			{
				Vec3f N(ray.Ng);
				N.normalise();
				Vec3f hitP(ray.org);
				hitP.scaleAdd(Vec3f(ray.dir), ray.tfar);
				Vec3f cf = m_geometry[ray.geomID]->shade(hitP, N, ray.u, ray.v);
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
