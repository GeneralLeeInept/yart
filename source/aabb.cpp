#include "aabb.h"

#include <algorithm>

Aabb::Aabb()
  : m_mins(std::numeric_limits<float>::max())
  , m_maxs(-std::numeric_limits<float>::max())
{
}

void Aabb::grow(const Vec3f& V)
{
	for (int i = 0; i < 3; ++i)
	{
		m_mins[i] = std::min(m_mins[i], V[i]);
		m_maxs[i] = std::max(m_maxs[i], V[i]);
	}
}

void Aabb::expand(float d)
{
	Vec3f D(d);
	m_mins = m_mins - D;
	m_maxs = m_maxs + D;
}
