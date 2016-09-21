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
	Vec3(double x, double y, double z);

	double lengthSq() const;
	double length() const;

	void normalise();
	void scale(double s);
	void scaleAdd(const Vec3& r, double s);

	static double dot(const Vec3& a, const Vec3& b);
	static Vec3 cross(const Vec3& a, const Vec3& b);

	union {
		double m_v[3];
		struct
		{
			double m_x;
			double m_y;
			double m_z;
		};
	};
};

Vec3 operator-(const Vec3& a, const Vec3& b);
Vec3 operator+(const Vec3& a, const Vec3& b);
