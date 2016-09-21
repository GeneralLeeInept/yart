#pragma once

class Vec3
{
public:
	static Vec3 UnitX;
	static Vec3 UnitY;
	static Vec3 UnitZ;
	static Vec3 Zero;
	static Vec3 One;

	Vec3();
	Vec3(float x, float y, float z);

	float lengthSq() const;
	float length() const;

	void normalise();
	void scale(float s);
	void scaleAdd(const Vec3& r, float s);

	static float dot(const Vec3& a, const Vec3& b);
	static Vec3 cross(const Vec3& a, const Vec3& b);

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

Vec3 operator-(const Vec3& a, const Vec3& b);
Vec3 operator+(const Vec3& a, const Vec3& b);
