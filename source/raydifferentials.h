#pragma once

#include "vec3f.h"

class RayDifferentials
{
public:
	RayDifferentials() = default;
	RayDifferentials(const Vec3f& right, const Vec3f& up, const Vec3f& d, const Vec3f& scale);

	void initialise(const Vec3f& right, const Vec3f& up, const Vec3f& d, const Vec3f& scale);
	void scale(float scale);
	void transfer(const Vec3f& D, const Vec3f& n, float t);
	void reflect(const Vec3f& D, const Vec3f& n);
	void refract(const Vec3f& D, const Vec3f& n, float eta);

	Vec3f dPdx, dPdy;
	Vec3f dDdx, dDdy;
};