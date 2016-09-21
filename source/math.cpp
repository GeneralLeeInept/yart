#include "math.h"

bool solveQuadratic(float a, float b, float c, float& x0, float& x1)
{
	float discr = b * b - 4.0f * a * c;
	if (discr < 0.0f)
	{
		return false;
	}
	else if (discr == 0.0f)
	{
		x0 = x1 = -0.5f * b / a;
	}
	else
	{
		float q = (b > 0.0f) ? -0.5f * (b + sqrtf(discr)) : -0.5f * (b - sqrtf(discr));
		x0 = q / a;
		x1 = c / q;
	}

	return true;
}
