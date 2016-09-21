#pragma once

#include "renderable.h"
#include "vec3.h"

class Ray;
class Vec2;

class Sphere : public IRenderable
{
public:
	Sphere();
	Sphere(const Vec3& centre, float radius);

	bool intersect(const Ray& ray, float& t) const;
	void getSurfaceData(const Vec3& Phit, Vec3& Nhit, Vec2& tex) const;

private:
	Vec3 m_centre;
	float m_radius;
};