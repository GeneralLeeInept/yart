#pragma once

#include <cinttypes>
#include "vec3.h"

class Colour : public Vec3
{
public:
	Colour();
	Colour(double r, double g, double b);

	void toBytes(uint8_t& r, uint8_t& g, uint8_t& b) const;
};