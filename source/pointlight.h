#pragma once

#include "light.h"

class PointLight : public Light
{
public:
	Vec3f L(const Vec3f& P) const;
	float intensity(const Vec3f& P) const;
	float distanceFrom(const Vec3f& P) const;

	//TODO falloff (just 1/r^2 now)
	Vec3f P;
	float m_intensity;
};