#include "ray.h"

Ray::Ray(const Vec3& origin, const Vec3& direction)
  : m_origin(origin)
  , m_direction(direction)
{
}
