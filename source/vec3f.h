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
	Vec3f(float f);
	Vec3f(float x, float y, float z);
	Vec3f(float* v);

	float lengthSq() const;
	float length() const;
	const float& operator[](size_t index) const;
	float& operator[](size_t index);

	void set(float x, float y, float z);
	void set(float xyz);
	void normalise();
	void scale(float s);
	void scaleAdd(const Vec3f& r, float s);

	static float dot(const Vec3f& a, const Vec3f& b);
	static Vec3f cross(const Vec3f& a, const Vec3f& b);

	union {
		float v[3];
		struct
		{
			float x;
			float y;
			float z;
		};
	};
};

Vec3f operator-(const Vec3f& a, const Vec3f& b);
Vec3f operator+(const Vec3f& a, const Vec3f& b);
Vec3f operator*(const Vec3f& a, const Vec3f& b);
