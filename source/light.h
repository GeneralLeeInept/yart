#pragma once

#include "colour.h"
#include "vec3.h"

class Light
{
public:
	Light(const Vec3& position, const Vec3& colour);

	Vec3 m_position;
	Vec3 m_colour;
};