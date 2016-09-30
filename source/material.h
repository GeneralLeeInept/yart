#pragma once

#include "shadable.h"
#include "vec3f.h"

class Material : IShadable
{
public:
	Vec3f Ka;
	Vec3f Kd;
	Vec3f Ks;
	float Ns;

	void shade(const Vec3f& P, const Vec3f& N, unsigned primId, float u, float v, Vec3f& colour) const;
};