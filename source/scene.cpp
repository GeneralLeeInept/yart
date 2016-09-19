#include "scene.h"

#include "ray.h"
#include "vec2.h"
#include <algorithm>

void Scene::addSphere(const Sphere& sphere)
{
	m_spheres.push_back(sphere);
}

void Scene::addLight(const Light& light)
{
	m_lights.push_back(light);
}

void Scene::castRay(const Ray& ray, Colour& colour) const
{
	double t;
	Sphere hit;

	if (trace(ray, t, hit))
	{
		Vec3 Phit;
		Vec3 Nhit;
		Vec2 tex;
		Phit = ray.m_origin;
		Phit.scaleAdd(ray.m_direction, t);
		hit.getSurfaceData(Phit, Nhit, tex);
		shade(ray, Phit, Nhit, tex, colour);
	}
	else
	{
		colour = Colour(0.0, 0.0, 0.0);
	}
}

bool Scene::trace(const Ray& ray, double& t, Sphere& hit) const
{
	t = std::numeric_limits<double>::max();

	for (auto sphere : m_spheres)
	{
		double t2 = std::numeric_limits<double>::max();
		if (sphere.intersect(ray, t2) && t2 < t && t2 > 0.01)
		{
			t = t2;
			hit = sphere;
		}
	}

	return (t < std::numeric_limits<double>::max());
}

void Scene::shade(const Ray& ray, const Vec3& position, const Vec3& normal, const Vec2& tex, Colour& colour) const
{
	double scale = 16.0;
	double pattern = (fmod(tex.m_x * scale, 1) > 0.5) ^ (fmod(tex.m_y * scale, 1) > 0.5);

	colour = Colour(0.0, 0.0, 0.0);

	for (auto light : m_lights)
	{
		Vec3 L = light.m_position - position;
		double d = L.lengthSq();
		L.normalise();
		Ray shadowRay(position, L);
		double t;
		Sphere hit;
		if (trace(shadowRay, t, hit))
		{
			if ((t * t) < d)
			{
				continue;
			}
		}
		colour.scaleAdd(light.m_colour, std::max(0.0, Vec3::dot(normal, L)));
	}
	colour.scale(pattern * 0.5 + 0.5);
}
