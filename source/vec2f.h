#pragma once

class Vec2f
{
public:
	static Vec2f UnitX;
	static Vec2f UnitY;
	static Vec2f Zero;

	Vec2f();
	Vec2f(float x, float y);

	union {
		float xy[2];
		struct
		{
			float x;
			float y;
		};
	};
};
