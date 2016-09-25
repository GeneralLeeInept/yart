#pragma once

class Aabb;
class Plane;
class Ray;
class Vec3f;

namespace geometry
{
void tests();
float pointToPlaneDistance(const Vec3f& V, const Plane& p);
int classifyTriangle(const Vec3f& V1, const Vec3f& V2, const Vec3f& V3, const Plane& p);
bool clip(const Ray& ray, const Aabb& aabb, float& tMin, float& tMax);
bool intersect(const Ray& ray, const Plane& p, float& t);
}