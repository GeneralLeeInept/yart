#include "scene.h"

#include "ray.h"
#include "renderable.h"
#include "vec2f.h"
#include "vec3f.h"
#include <algorithm>

void Scene::addObject(IRenderable* object)
{
	m_objects.push_back(std::unique_ptr<IRenderable>(object));
}

void Scene::addLight(const Light& light)
{
	m_lights.push_back(light);
}

void Scene::castRay(const Ray& ray, Colour& colour) const
{
	float t;
	HitData hitData;

	if (trace(ray, t, hitData))
	{
		hitData.m_hitPosition = ray.m_origin;
		hitData.m_hitPosition.scaleAdd(ray.m_direction, t);
		hitData.m_hitObject->getSurfaceData(hitData);
		Vec3f result(1.0f);
		//shade(ray, hitData, result);
		colour = Colour(result);
	}
	else
	{
		colour = Colour();
	}
}

bool Scene::trace(const Ray& ray, float& t) const
{
	HitData unused;
	return trace(ray, t, unused);
}

bool Scene::trace(const Ray& ray, float& t, HitData& hitData) const
{
	t = std::numeric_limits<float>::max();
	hitData.m_hitObject = nullptr;
	for (auto& object : m_objects)
	{
		float t2 = std::numeric_limits<float>::max();
		if (object->intersect(ray, t2) && t2 < t && t2 > 0.001)
		{
			t = t2;
			hitData.m_hitObject = object.get();
		}
	}

	return (hitData.m_hitObject != nullptr);
}

void Scene::shade(const Ray& ray, const HitData& hitData, Vec3f& result) const
{
	float scale = 16.0f;
	int pattern =
	  (fmodf(hitData.m_hitUV.x * scale, 1.0f) > 0.5f) ^ (fmodf(hitData.m_hitUV.y * scale, 1.0f) > 0.5f);
	
	result = Vec3f::Zero;

	for (auto light : m_lights)
	{
		Vec3f L = light.m_position - hitData.m_hitPosition;
		float d = L.lengthSq();
		L.normalise();
		Ray shadowRay(hitData.m_hitPosition, L);
		float t;
		if (trace(shadowRay, t))
		{
			if ((t * t) < d)
			{
				continue;
			}
		}
		result.scaleAdd(light.m_colour, std::max(0.0f, Vec3f::dot(hitData.m_hitNormal, L)));
	}
	result.scale(pattern * 0.5f + 0.5f);
}
