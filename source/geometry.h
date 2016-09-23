#pragma once

class Plane;
class Vec3f;

namespace geometry
{
void tests();
float pointToPlaneDistance(const Vec3f& V, const Plane& p);
}