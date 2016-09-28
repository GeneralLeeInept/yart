#pragma once

#include "vec3f.h"

class Matrix33
{
public:
	Matrix33();
	Matrix33(const Vec3f& x, const Vec3f& y, const Vec3f& z);

	void rotateY(float radians);

	Vec3f operator*(const Vec3f& V) const;
	Matrix33 operator*(const Matrix33& M) const;

	Vec3f& operator[](size_t column);

	union {
		Vec3f m[3];
		struct
		{
			Vec3f X;
			Vec3f Y;
			Vec3f Z;
		};
	};
};