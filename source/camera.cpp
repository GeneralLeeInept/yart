#include "camera.h"

Ray Camera::createRay(double u, double v) const
{
	Vec3 viewPlanePosition(u, v, 1.0);
	Vec3 direction = viewPlanePosition - m_position;
	direction.normalise();
	return Ray(m_position, direction);
}
