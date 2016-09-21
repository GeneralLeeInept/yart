#include "camera.h"

Ray Camera::createRay(float u, float v) const
{
	Vec3 direction(u, v, 1.0);
	direction.normalise();
	return Ray(m_position, direction);
}
