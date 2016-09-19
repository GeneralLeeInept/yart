#include "math.h"

bool solveQuadratic(double a, double b, double c, double& x0, double& x1)
{
	double discr = b * b - 4.0 * a * c;
	if (discr < 0.0)
	{
		return false;
	}
	else if (discr == 0.0)
	{
		x0 = x1 = -0.5 * b / a;
	}
	else
	{
		double q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}

	return true;
}
