#pragma once

#include "vec3f.h"

class Light
{
public:
	virtual ~Light() = default;

	virtual Vec3f L(const Vec3f& P) const = 0;
	virtual float intensity(const Vec3f& P) const = 0;

	Vec3f m_colour;
};