#pragma once

class Vec2
{
public:
	static Vec2 UnitX;
	static Vec2 UnitY;
	static Vec2 Zero;

	Vec2();
	Vec2(double x, double y);

	union {
		double m_v[2];
		struct
		{
			double m_x;
			double m_y;
		};
	};
};
