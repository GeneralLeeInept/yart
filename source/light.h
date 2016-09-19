#pragma once

#include "colour.h"
#include "vec3.h"

class Light
{
public:
	Light(const Vec3& position, const Colour& colour);

	Vec3 m_position;
	Colour m_colour;
};