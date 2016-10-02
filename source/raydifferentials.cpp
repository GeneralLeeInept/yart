#include <cmath>
#include "raydifferentials.h"

RayDifferentials::RayDifferentials(const Vec3f& right, const Vec3f& up, const Vec3f& d, const Vec3f& scale)
{
	initialise(right, up, d, scale);
}

void RayDifferentials::initialise(const Vec3f& right, const Vec3f& up, const Vec3f& d, const Vec3f& scale)
{
	// Equation (8) of [Igehy 1999]
	float norm = Vec3f::dot(d, d);
	float invPowNorm = 1.0f / std::pow(norm, 1.5f);
	dDdx = scale[0] * (norm * right - Vec3f::dot(d, right) * d);
	dDdx.scale(invPowNorm);
	dDdy = scale[1] * (norm * up - Vec3f::dot(d, up) * d);
	dDdy.scale(invPowNorm);
}

void RayDifferentials::scale(float scale)
{
	dPdx.scale(scale);
	dPdy.scale(scale);
	dDdx.scale(scale);
	dDdy.scale(scale);
}

void RayDifferentials::transfer(const Vec3f& D, const Vec3f& n, float t)
{
	Vec3f fn = n;
	if (Vec3f::dot(fn, D) > 0.f)
		fn.negate();

	// Equation (10-12) of [Igehy 1999]
	float rDdotN = 1.0f / Vec3f::dot(D, fn);
	float dtdx = Vec3f::dot(-(dPdx + t * dDdx), fn) * rDdotN;
	dPdx = dPdx + t * dDdx + dtdx * D;
	float dtdy = Vec3f::dot(-(dPdy + t * dDdy), fn) * rDdotN;
	dPdy = dPdy + t * dDdy + dtdy * D;
	// dDdx/dDdy remain unchanged
}

void RayDifferentials::reflect(const Vec3f& D, const Vec3f& n)
{
	// TODO: implement it
}

void RayDifferentials::refract(const Vec3f& D, const Vec3f& n, float eta)
{
	// TODO: implement it
}
