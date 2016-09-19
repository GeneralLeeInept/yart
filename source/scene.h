#pragma once

#include <vector>

#include "light.h"
#include "sphere.h"

class Ray;
class Sphere;
class Vec2;
class Vec3;

class Scene
{
public:
	void addSphere(const Sphere& sphere);
	void addLight(const Light& light);

	void castRay(const Ray& ray, Colour& colour) const;
	bool trace(const Ray& ray, double& t, Sphere& hit) const;
	void shade(const Ray& ray, const Vec3& position, const Vec3& normal, const Vec2& tex, Colour& colour) const;

private:
	std::vector<Sphere> m_spheres;
	std::vector<Light> m_lights;
};
