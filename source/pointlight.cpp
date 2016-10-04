#include "pointlight.h"

#include "math.h"
#include <algorithm>

Vec3f PointLight::L(const Vec3f& P) const
{
	Vec3f l = this->P - P;
	l.normalise();
	return l;
}

float PointLight::intensity(const Vec3f& P) const
{
	Vec3f l = this->P - P;
	float d = l.lengthSq();
	float denom = 4.0f * static_cast<float>(M_PI) * d;
	return (m_intensity * m_intensity) / denom;
}

float PointLight::distanceFrom(const Vec3f & P) const
{
	return (this->P - P).length();
}

