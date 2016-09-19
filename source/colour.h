#pragma once

#include <cinttypes>

class Colour
{
public:
	Colour();
	Colour(double r, double g, double b);

	void toBytes(uint8_t& r, uint8_t& g, uint8_t& b) const;

	union {
		double m_v[3];
		struct
		{
			double m_r;
			double m_g;
			double m_b;
		};
	};
};