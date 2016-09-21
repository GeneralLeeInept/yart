#include "scene.h"

#include "ray.h"
#include "renderable.h"
#include "vec2.h"
#include "vec3.h"
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
	IRenderable* hit;

	if (trace(ray, t, hit))
	{
		Vec3 Phit;
		Vec3 Nhit;
		Vec2 tex;
		Phit = ray.m_origin;
		Phit.scaleAdd(ray.m_direction, t);
		hit->getSurfaceData(Phit, Nhit, tex);
		shade(ray, Phit, Nhit, tex, colour);
		//colour = Colour(255, 255, 255);
	}
	else
	{
		colour = Colour();
	}
}

bool Scene::trace(const Ray& ray, float& t, IRenderable*& hit) const
{
	t = std::numeric_limits<float>::max();
	hit = nullptr;
	for (auto& object : m_objects)
	{
		float t2 = std::numeric_limits<float>::max();
		if (object->intersect(ray, t2) && t2 < t && t2 > 0.001)
		{
			t = t2;
			hit = object.get();
		}
	}

	return (hit != nullptr);
}

void Scene::shade(const Ray& ray, const Vec3& position, const Vec3& normal, const Vec2& tex, Colour& colour) const
{
	float scale = 16.0f;
	int pattern = (fmodf(tex.x * scale, 1.0f) > 0.5f) ^ (fmodf(tex.y * scale, 1.0f) > 0.5f);
	Vec3 colourf;

	for (auto light : m_lights)
	{
		Vec3 L = light.m_position - position;
		float d = L.lengthSq();
		L.normalise();
		Ray shadowRay(position, L);
		float t;
		IRenderable* hit;
		if (trace(shadowRay, t, hit))
		{
			if ((t * t) < d)
			{
				continue;
			}
		}
		colourf.scaleAdd(light.m_colour, std::max(0.0f, Vec3::dot(normal, L)));
	}
	colourf.scale(pattern * 0.5f + 0.5f);
	colour = Colour(colourf);
}
