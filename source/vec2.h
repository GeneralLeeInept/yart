#pragma once

class Vec2
{
public:
	static Vec2 UnitX;
	static Vec2 UnitY;
	static Vec2 Zero;

	Vec2();
	Vec2(float x, float y);

	union {
		float xy[2];
		struct
		{
			float x;
			float y;
		};
	};
};
