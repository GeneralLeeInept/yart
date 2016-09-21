#include "camera.h"

Ray Camera::createRay(double u, double v) const
{
	Vec3 direction(u, v, 1.0);
	direction.normalise();
	return Ray(m_position, direction);
}
