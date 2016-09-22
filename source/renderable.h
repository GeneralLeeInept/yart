#pragma once

#include "Ray.h"
#include "Vec2f.h"
#include "Vec3f.h"

struct HitData
{
	class IRenderable* m_hitObject;
	Vec3f m_hitPosition;
	Vec3f m_hitNormal;
	Vec2f m_hitUV;
	Vec3f m_hitColour;
};

class IRenderable
{
public:
	virtual ~IRenderable() = default;

	virtual bool intersect(const Ray& ray, float& t) const = 0;
	virtual void getSurfaceData(HitData& hitData) const = 0;
};