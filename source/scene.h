#pragma once

#include <vector>

#include "light.h"
#include "ray.h"
#include "sphere.h"

class Scene
{
public:
	void addSphere(const Sphere& sphere);
	void addLight(const Light& light);

	void castRay(const Ray& ray, Colour& colour) const;
	bool trace(const Ray& ray, double& t, Sphere& hit) const;

private:
	std::vector<Sphere> m_spheres;
	std::vector<Light> m_lights;
};
