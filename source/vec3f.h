#pragma once

class Vec3f
{
public:
	static Vec3f UnitX;
	static Vec3f UnitY;
	static Vec3f UnitZ;
	static Vec3f Zero;
	static Vec3f One;

	Vec3f();
	Vec3f(float x, float y, float z);

	float lengthSq() const;
	float length() const;

	void normalise();
	void scale(float s);
	void scaleAdd(const Vec3f& r, float s);

	static float dot(const Vec3f& a, const Vec3f& b);
	static Vec3f cross(const Vec3f& a, const Vec3f& b);

	union {
		float xyz[3];
		struct
		{
			float m_x;
			float m_y;
			float m_z;
		};
	};
};

Vec3f operator-(const Vec3f& a, const Vec3f& b);
Vec3f operator+(const Vec3f& a, const Vec3f& b);
