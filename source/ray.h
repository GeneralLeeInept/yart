#pragma once

#include "vec3f.h"

class Ray
{
public:
	Ray(const Vec3f& origin, const Vec3f& direction);

	Vec3f O;
	Vec3f D;
};
