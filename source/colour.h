#pragma once

#include <cinttypes>

class Vec3;

class Colour
{
public:
	Colour();
	Colour(const Vec3& v);
	Colour(uint8_t r, uint8_t g, uint8_t b);

	union
	{
		uint8_t rgb[3];
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};
	};
};