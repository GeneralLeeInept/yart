#include "geometry.h"

#include "aabb.h"
#include "math.h"
#include "plane.h"
#include "ray.h"
#include "vec3f.h"
#include <algorithm>
#include <cassert>

void geometry::tests()
{
	Vec3f V(1.0f, 1.0f, 1.0f);
	Plane p(Vec3f::UnitY, 0.0f);
	assert(compareFloats(pointToPlaneDistance(V, p), 1.0f) == 0);
	p.N.scale(-1.0f);
	assert(compareFloats(pointToPlaneDistance(V, p), -1.0f) == 0);
}

float geometry::pointToPlaneDistance(const Vec3f& V, const Plane& p)
{
	float proj = Vec3f::dot(V, p.N);
	return proj - p.d;
}

int geometry::classifyTriangle(const Vec3f& V1, const Vec3f& V2, const Vec3f& V3, const Plane& p)
{
	int cv1 = compareFloats(pointToPlaneDistance(V1, p), 0.0f);
	int cv2 = compareFloats(pointToPlaneDistance(V2, p), 0.0f);
	int cv3 = compareFloats(pointToPlaneDistance(V3, p), 0.0f);

	if ((!cv1 || cv1 == cv2) && cv2 == cv3)
	{
		// All on one side, or lies in plane
		// Could call out lies in plane specially (it presents the same as straddles)
		return cv2;
	}

	// Straddles plane
	return 0;
}

bool geometry::clip(const Ray& ray, const Aabb& aabb, float& tMin, float& tMax)
{
	for (int i = 0; i < 3; ++i)
	{
		if (compareFloats(ray.D[i], 0.0f))
		{
			float t1 = (aabb.m_mins[i] - ray.O[i]) / ray.D[i];
			float t2 = (aabb.m_maxs[i] - ray.O[i]) / ray.D[i];

			tMin = std::max(tMin, std::min(t1, t2));
			tMax = std::min(tMax, std::max(t1, t2));
		}
		else if (ray.O[i] < aabb.m_mins[i] || ray.O[i] >= aabb.m_maxs[i])
		{
			return false;
		}
	}

	return true;
}
