#pragma once

#include "vec3.h"

class Matrix33
{
public:
	Matrix33();
	Matrix33(const Vec3& x, const Vec3& y, const Vec3& z);

	union {
		Vec3 m_m[3];
		struct
		{
			Vec3 m_x;
			Vec3 m_y;
			Vec3 m_z;
		};
	};
};