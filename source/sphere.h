#pragma once

#include "renderable.h"
#include "vec3f.h"

class Ray;
class Vec2;

class Sphere : public IRenderable
{
public:
	Sphere();
	Sphere(const Vec3f& centre, float radius);

	bool intersect(const Ray& ray, float& t) const;
	void getSurfaceData(const Vec3f& Phit, Vec3f& Nhit, Vec2& tex) const;

private:
	Vec3f m_centre;
	float m_radius;
};