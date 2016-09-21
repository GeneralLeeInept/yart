#include "ray.h"

Ray::Ray(const Vec3f& origin, const Vec3f& direction)
  : m_origin(origin)
  , m_direction(direction)
{
}
