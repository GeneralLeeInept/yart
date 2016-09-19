#include "scene.h"

#include <algorithm>
#include "vec2.h"

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
		double scale = 4.0;
		double pattern = (fmod(tex.m_x * scale, 1) > 0.5) ^ (fmod(tex.m_y * scale, 1) > 0.5);
		double c = std::max(0.0, -Vec3::dot(Nhit, ray.m_direction)) * pattern;
		colour = Colour(c, c, c);
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
		if (sphere.intersect(ray, t2) && t2 < t)
		{
			t = t2;
			hit = sphere;
		}
	}

	return (t < std::numeric_limits<double>::max());
}