#include "matrix33.h"

Matrix33::Matrix33()
  : Matrix33(Vec3::UnitX, Vec3::UnitY, Vec3::UnitZ)
{
}

Matrix33::Matrix33(const Vec3& x, const Vec3& y, const Vec3& z)
  : m_x(x)
  , m_y(y)
  , m_z(z)
{
}
