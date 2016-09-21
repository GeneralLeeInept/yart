#pragma once

class Ray;
class Vec2;
class Vec3f;

class IRenderable
{
public:
	virtual ~IRenderable() = default;

	virtual bool intersect(const Ray& ray, float& t) const = 0;
	virtual void getSurfaceData(const Vec3f& Phit, Vec3f& Nhit, Vec2& tex) const = 0;
};