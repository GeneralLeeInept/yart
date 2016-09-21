#include <math.h>

#include "vec3f.h"

Vec3f Vec3f::UnitX(1.0, 0.0, 0.0);
Vec3f Vec3f::UnitY(0.0, 1.0, 0.0);
Vec3f Vec3f::UnitZ(0.0, 0.0, 1.0);
Vec3f Vec3f::Zero(0.0, 0.0, 0.0);
Vec3f Vec3f::One(1.0, 1.0, 1.0);

Vec3f::Vec3f()
  : Vec3f(Zero)
{
}

Vec3f::Vec3f(float x, float y, float z)
  : m_x(x)
  , m_y(y)
  , m_z(z)
{
}

float Vec3f::lengthSq() const
{
	return dot(*this, *this);
}

float Vec3f::length() const
{
	return sqrtf(lengthSq());
}

void Vec3f::normalise()
{
	scale(1.0f / length());
}

void Vec3f::scale(float s)
{
	for (int i = 0; i < 3; ++i)
	{
		xyz[i] *= s;
	}
}

void Vec3f::scaleAdd(const Vec3f& r, float s)
{
	Vec3f rs = r;
	rs.scale(s);
	*this = *this + rs;
}

float Vec3f::dot(const Vec3f& a, const Vec3f& b)
{
	float dp = 0.f;
	for (int i = 0; i < 3; ++i)
	{
		dp += a.xyz[i] * b.xyz[i];
	}
	return dp;
}

Vec3f Vec3f::cross(const Vec3f& a, const Vec3f& b)
{
	float x = a.m_y * b.m_z - a.m_z * b.m_y;
	float y = a.m_z * b.m_x - a.m_x * b.m_z;
	float z = a.m_x * b.m_y - a.m_y * b.m_x;
	return Vec3f(x, y, z);
}

Vec3f operator-(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
}

Vec3f operator+(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
}
