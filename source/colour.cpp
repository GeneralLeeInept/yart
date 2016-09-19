#include "colour.h"

Colour::Colour()
  : Colour(0.0, 0.0, 0.0)
{
}

Colour::Colour(double r, double g, double b)
  : m_r(r)
  , m_g(g)
  , m_b(b)
{
}

void Colour::toBytes(uint8_t& r, uint8_t& g, uint8_t& b) const
{
	r = static_cast<uint8_t>(((m_r < 0.0) ? 0.0 : ((m_r > 1.0) ? 1.0 : m_r)) * 255.0);
	g = static_cast<uint8_t>(((m_g < 0.0) ? 0.0 : ((m_g > 1.0) ? 1.0 : m_g)) * 255.0);
	b = static_cast<uint8_t>(((m_b < 0.0) ? 0.0 : ((m_b > 1.0) ? 1.0 : m_b)) * 255.0);
}
