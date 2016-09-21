#include <math.h>

#include "vec3.h"

Vec3 Vec3::UnitX(1.0, 0.0, 0.0);
Vec3 Vec3::UnitY(0.0, 1.0, 0.0);
Vec3 Vec3::UnitZ(0.0, 0.0, 1.0);
Vec3 Vec3::Zero(0.0, 0.0, 0.0);
Vec3 Vec3::One(1.0, 1.0, 1.0);

Vec3::Vec3()
  : Vec3(Zero)
{
}

Vec3::Vec3(float x, float y, float z)
  : m_x(x)
  , m_y(y)
  , m_z(z)
{
}

float Vec3::lengthSq() const
{
	return dot(*this, *this);
}

float Vec3::length() const
{
	return sqrtf(lengthSq());
}

void Vec3::normalise()
{
	scale(1.0f / length());
}

void Vec3::scale(float s)
{
	for (int i = 0; i < 3; ++i)
	{
		xyz[i] *= s;
	}
}

void Vec3::scaleAdd(const Vec3& r, float s)
{
	Vec3 rs = r;
	rs.scale(s);
	*this = *this + rs;
}

float Vec3::dot(const Vec3& a, const Vec3& b)
{
	float dp = 0.f;
	for (int i = 0; i < 3; ++i)
	{
		dp += a.xyz[i] * b.xyz[i];
	}
	return dp;
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b)
{
	float x = a.m_y * b.m_z - a.m_z * b.m_y;
	float y = a.m_z * b.m_x - a.m_x * b.m_z;
	float z = a.m_x * b.m_y - a.m_y * b.m_x;
	return Vec3(x, y, z);
}

Vec3 operator-(const Vec3& a, const Vec3& b)
{
	return Vec3(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
}

Vec3 operator+(const Vec3& a, const Vec3& b)
{
	return Vec3(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
}
