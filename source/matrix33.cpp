#include "matrix33.h"

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
