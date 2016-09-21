#pragma once

class Ray;
class Vec2;
class Vec3;

class IRenderable
{
public:
	virtual ~IRenderable() = default;

	virtual bool intersect(const Ray& ray, float& t) const = 0;
	virtual void getSurfaceData(const Vec3& Phit, Vec3& Nhit, Vec2& tex) const = 0;
};