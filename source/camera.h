#pragma once

#include "matrix33.h"
#include "ray.h"
#include "vec3.h"

class Camera
{
public:
	Ray createRay(double u, double v) const;

	Matrix33 m_orientation;
	Vec3 m_position;
};