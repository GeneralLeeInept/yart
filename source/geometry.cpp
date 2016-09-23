#include "geometry.h"

#include <cassert>
#include "math.h"
#include "plane.h"
#include "vec3f.h"

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
