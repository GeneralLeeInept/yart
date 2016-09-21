#pragma once

#include "colour.h"
#include "vec3f.h"

class Light
{
public:
	Light(const Vec3f& position, const Vec3f& colour);

	Vec3f m_position;
	Vec3f m_colour;
};