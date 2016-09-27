#pragma once

#include "matrix33.h"
#include "vec3f.h"

struct RTCRay;

class Camera
{
public:
	void createRay(RTCRay& ray, float u, float v) const;

	Matrix33 m_orientation;
	Vec3f m_position;
};