#pragma once

#include <limits>
#define _USE_MATH_DEFINES
#include <math.h>

bool solveQuadratic(float a, float b, float c, float& x0, float& x1);
int compareFloats(float a, float b, float epsilon = std::numeric_limits<float>::epsilon());