#pragma once

class RayDifferentials;
class Vec3f;
struct RTCRay;

class IShadable
{
public:
	virtual ~IShadable() = default;

	virtual void shade(const Vec3f& P, const Vec3f& N, const RTCRay& ray, Vec3f& colour,
	                   RayDifferentials& rd) const = 0;
};