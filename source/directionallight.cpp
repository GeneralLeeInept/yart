#include "directionallight.h"

Vec3f DirectionalLight::L(const Vec3f & P) const
{
	return -D;
}

float DirectionalLight::intensity(const Vec3f & P) const
{
	return 1.0f;
}
