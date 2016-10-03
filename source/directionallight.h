#pragma once

#include "light.h"

class DirectionalLight : public Light
{
public:
	Vec3f L(const Vec3f& P) const;
	float intensity(const Vec3f& P) const;

	Vec3f D;
};