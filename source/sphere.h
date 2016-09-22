#pragma once

#include "renderable.h"
#include "vec3f.h"

class Ray;
class Vec2f;

class Sphere : public IRenderable
{
public:
	Sphere();
	Sphere(const Vec3f& centre, float radius);

	bool intersect(const Ray& ray, float& t) const;
	void getSurfaceData(HitData& hitData) const;

private:
	Vec3f m_centre;
	float m_radius;
};