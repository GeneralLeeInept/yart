#pragma once

#include "vec3f.h"

class Ray
{
public:
	Ray(const Vec3f& origin, const Vec3f& direction);

	Vec3f m_origin;
	Vec3f m_direction;
};
