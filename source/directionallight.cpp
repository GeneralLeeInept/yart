#include "directionallight.h"

#include <limits>

Vec3f DirectionalLight::L(const Vec3f& P) const
{
	return -D;
}

float DirectionalLight::intensity(const Vec3f& P) const
{
	return 1.0f;
}

float DirectionalLight::distanceFrom(const Vec3f& P) const
{
	return std::numeric_limits<float>::infinity();
}
