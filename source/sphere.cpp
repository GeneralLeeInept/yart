#include "sphere.h"

#include "math.h"
#include "ray.h"
#include "vec2.h"
#include <algorithm>

Sphere::Sphere()
  : Sphere(Vec3::Zero, 1.0)
{
}

Sphere::Sphere(const Vec3& centre, float radius)
  : m_centre(centre)
  , m_radius(radius)
{
}

bool Sphere::intersect(const Ray& ray, float& t) const
{
	float t0, t1; // solutions for t if the ray intersects
	Vec3 L = ray.m_origin - m_centre;
	float a = Vec3::dot(ray.m_direction, ray.m_direction);
	float b = 2 * Vec3::dot(ray.m_direction, L);
	float c = L.lengthSq() - (m_radius * m_radius);
	if (!solveQuadratic(a, b, c, t0, t1))
		return false;

	if (t0 > t1)
		std::swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead
		if (t0 < 0)
			return false; // both t0 and t1 are negative
	}

	t = t0;

	return true;
}

void Sphere::getSurfaceData(const Vec3& Phit, Vec3& Nhit, Vec2& tex) const
{
	Nhit = Phit - m_centre;
	Nhit.normalise();
	float pi = static_cast<float>(M_PI);
	tex.x = (1.0f + atan2f(Nhit.m_z, Nhit.m_x) / pi) * 0.5f;
	tex.y = acosf(Nhit.m_y) / pi;
}