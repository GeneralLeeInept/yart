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

Vec3f::Vec3f(float f)
  : Vec3f(f, f, f)
{
}

Vec3f::Vec3f(float x, float y, float z)
  : x(x)
  , y(y)
  , z(z)
{
}

Vec3f::Vec3f(const float* v)
  : x(v[0])
  , y(v[1])
  , z(v[2])
{
}

Vec3f& Vec3f::operator+=(Vec3f b)
{
	*this = *this + b;
	return *this;
}

float Vec3f::lengthSq() const
{
	return dot(*this, *this);
}

float Vec3f::length() const
{
	return sqrtf(lengthSq());
}

const float& Vec3f::operator[](size_t index) const
{
	return v[index];
}

float& Vec3f::operator[](size_t index)
{
	return v[index];
}

void Vec3f::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vec3f::set(float xyz)
{
	set(xyz, xyz, xyz);
}

void Vec3f::normalise()
{
	scale(1.0f / length());
}

void Vec3f::scale(float s)
{
	for (int i = 0; i < 3; ++i)
	{
		v[i] *= s;
	}
}

void Vec3f::scaleAdd(const Vec3f& r, float s)
{
	Vec3f rs = r;
	rs.scale(s);
	*this = *this + rs;
}

void Vec3f::negate()
{
	for (size_t i = 0; i < 3; ++i)
		v[i] = -v[i];
}

float Vec3f::dot(const Vec3f& a, const Vec3f& b)
{
	float dp = 0.f;
	for (int i = 0; i < 3; ++i)
	{
		dp += a[i] * b[i];
	}
	return dp;
}

Vec3f Vec3f::cross(const Vec3f& a, const Vec3f& b)
{
	float x = a.y * b.z - a.z * b.y;
	float y = a.z * b.x - a.x * b.z;
	float z = a.x * b.y - a.y * b.x;
	return Vec3f(x, y, z);
}

Vec3f operator-(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3f operator-(const Vec3f& a)
{
	return Vec3f(-a.x, -a.y, -a.z);
}

Vec3f operator+(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3f operator*(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}
