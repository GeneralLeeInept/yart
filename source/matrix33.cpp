#include "matrix33.h"

#include "math.h"

Matrix33::Matrix33()
  : Matrix33(Vec3f::UnitX, Vec3f::UnitY, Vec3f::UnitZ)
{
}

Matrix33::Matrix33(const Vec3f& x, const Vec3f& y, const Vec3f& z)
  : X(x)
  , Y(y)
  , Z(z)
{
}

void Matrix33::rotateY(float radians)
{
	Matrix33 rotation;
	float s = sinf(radians);
	float c = cosf(radians);
	rotation.X = Vec3f(c, 0, -s);
	rotation.Z = Vec3f(s, 0, c);
	*this = *this * rotation;
}

Vec3f Matrix33::operator*(const Vec3f& V) const
{
	return Vec3f(Vec3f::dot(X, V), Vec3f::dot(Y, V), Vec3f::dot(Z, V));
}

Matrix33 Matrix33::operator*(const Matrix33& M) const
{
	Vec3f R[3];
	for (int i = 0; i < 3; ++i)
		R[i] = Vec3f(X[i], Y[i], Z[i]);

	Matrix33 result;

	for (int i = 0; i < 3; ++i)
		result[i] = Vec3f(Vec3f::dot(R[i], M.X), Vec3f::dot(R[i], M.Y), Vec3f::dot(R[i], M.Z));

	return result;
}

Vec3f& Matrix33::operator[](size_t column)
{
	return m[column];
}
