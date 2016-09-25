#pragma once

#include "vec3f.h"

class Aabb
{
public:
	Aabb();

	void grow(const Vec3f& V);
	void expand(float d);

	Vec3f m_mins;
	Vec3f m_maxs;
};