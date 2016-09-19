#pragma once

#include "vec3.h"

class Ray;
class Vec2;

class Sphere
{
public:
	Sphere();
	Sphere(const Vec3& centre, double radius);

	bool intersect(const Ray& ray, double& t) const;
	void getSurfaceData(const Vec3& Phit, Vec3& Nhit, Vec2& tex) const;

private:
	Vec3 m_centre;
	double m_radius;
};