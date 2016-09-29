#pragma once

class Vec3f;

class IShadable
{
public:
	virtual ~IShadable() = default;

	virtual void shade(const Vec3f& P, const Vec3f& N, unsigned primId, float u, float v, Vec3f& colour) const = 0;
};