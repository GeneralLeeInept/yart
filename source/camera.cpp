#include "camera.h"
#include <embree2/rtcore_ray.h>
#include <limits>

void Camera::createRay(RTCRay& ray, float u, float v) const
{
	Vec3f direction(u, v, 1.0);
	direction.normalise();
	
	for (int i = 0; i < 3; ++i)
	{
		ray.org[i] = m_position[i];
		ray.dir[i] = direction[i];
	}

	ray.tnear = std::numeric_limits<float>::epsilon();
	ray.tfar = std::numeric_limits<float>::infinity();
	ray.instID = RTC_INVALID_GEOMETRY_ID;
	ray.geomID = RTC_INVALID_GEOMETRY_ID;
	ray.primID = RTC_INVALID_GEOMETRY_ID;
	ray.mask = 0xffffffff;
	ray.time = 0.0f;
}
