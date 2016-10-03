#pragma once

class Light;
class RayDifferentials;
class Vec3f;
struct RTCRay;

class IShadable
{
public:
	virtual ~IShadable() = default;

	virtual Vec3f shade(const Vec3f& P, const Vec3f& N, const RTCRay& ray, const Light& light,
	                   RayDifferentials& rd) const = 0;
};