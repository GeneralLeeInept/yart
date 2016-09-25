#include "ray.h"

Ray::Ray(const Vec3f& origin, const Vec3f& direction)
  : O(origin)
  , D(direction)
{
}
