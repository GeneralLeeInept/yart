#pragma once

#include <memory>
#include <vector>

#include "light.h"
#include "sphere.h"
#include "vec2f.h"
#include "vec3f.h"

class Ray;
class IRenderable;
struct HitData;

class Scene
{
public:
	void addObject(IRenderable* object);
	void addLight(const Light& light);

	void castRay(const Ray& ray, Colour& colour) const;

private:
	bool trace(const Ray& ray, float& t) const;
	bool trace(const Ray& ray, float& t, HitData& hitData) const;
	void shade(const Ray& ray, const HitData& hitData, Vec3f& result) const;

private:
	std::vector<std::unique_ptr<IRenderable>> m_objects;
	std::vector<Light> m_lights;
};
