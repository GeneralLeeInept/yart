#include "sphere.h"

#include "math.h"
#include "ray.h"
#include "vec2f.h"
#include <algorithm>

Sphere::Sphere()
  : Sphere(Vec3f::Zero, 1.0)
{
}

Sphere::Sphere(const Vec3f& centre, float radius)
  : m_centre(centre)
  , m_radius(radius)
{
}

bool Sphere::intersect(const Ray& ray, float& t) const
{
	float t0, t1; // solutions for t if the ray intersects
	Vec3f L = ray.O - m_centre;
	float a = Vec3f::dot(ray.D, ray.D);
	float b = 2 * Vec3f::dot(ray.D, L);
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

void Sphere::getSurfaceData(HitData& hitData) const
{
	hitData.m_hitNormal = hitData.m_hitPosition - m_centre;
	hitData.m_hitNormal.normalise();
	float pi = static_cast<float>(M_PI);
	hitData.m_hitUV.x = (1.0f + atan2f(hitData.m_hitNormal.z, hitData.m_hitNormal.x) / pi) * 0.5f;
	hitData.m_hitUV.y = acosf(hitData.m_hitNormal.y) / pi;
}