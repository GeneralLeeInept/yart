#pragma once

#include "vec3f.h"

class Matrix33
{
public:
	Matrix33();
	Matrix33(const Vec3f& x, const Vec3f& y, const Vec3f& z);

	union {
		Vec3f m_m[3];
		struct
		{
			Vec3f m_x;
			Vec3f m_y;
			Vec3f m_z;
		};
	};
};