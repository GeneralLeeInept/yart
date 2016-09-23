#pragma once

#include "vec3f.h"

class Plane
{
public:
	Plane() = default;
	Plane(const Vec3f& N, float d);

	Vec3f N;
	float d;
};