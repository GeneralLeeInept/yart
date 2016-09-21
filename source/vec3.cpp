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

Vec3::Vec3(double x, double y, double z)
  : m_x(x)
  , m_y(y)
  , m_z(z)
{
}

double Vec3::lengthSq() const
{
	return dot(*this, *this);
}

double Vec3::length() const
{
	return sqrt(lengthSq());
}

void Vec3::normalise()
{
	scale(1.0 / length());
}

void Vec3::scale(double s)
{
	for (int i = 0; i < 3; ++i)
	{
		m_v[i] *= s;
	}
}

void Vec3::scaleAdd(const Vec3& r, double s)
{
	Vec3 rs = r;
	rs.scale(s);
	*this = *this + rs;
}

double Vec3::dot(const Vec3& a, const Vec3& b)
{
	double dp = 0.f;
	for (int i = 0; i < 3; ++i)
	{
		dp += a.m_v[i] * b.m_v[i];
	}
	return dp;
}

Vec3 Vec3::cross(const Vec3 & a, const Vec3 & b)
{
	double x = a.m_y * b.m_z - a.m_z * b.m_y;
	double y = a.m_z * b.m_x - a.m_x * b.m_z;
	double z = a.m_x * b.m_y - a.m_y * b.m_x;
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
