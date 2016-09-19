#pragma once

#include "vec3.h"

class Ray
{
public:
	Ray(const Vec3& origin, const Vec3& direction);

	Vec3 m_origin;
	Vec3 m_direction;
};
