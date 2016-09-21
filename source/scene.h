#pragma once

#include <memory>
#include <vector>

#include "light.h"
#include "sphere.h"

class Ray;
class IRenderable;
class Vec2;
class Vec3f;

class Scene
{
public:
	void addObject(IRenderable* object);
	void addLight(const Light& light);

	void castRay(const Ray& ray, Colour& colour) const;
	bool trace(const Ray& ray, float& t, IRenderable*& hit) const;
	void shade(const Ray& ray, const Vec3f& position, const Vec3f& normal, const Vec2& tex, Colour& colour) const;

private:
	std::vector<std::unique_ptr<IRenderable>> m_objects;
	std::vector<Light> m_lights;
};
