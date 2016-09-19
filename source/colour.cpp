#include "colour.h"

Colour::Colour()
  : Colour(0.0, 0.0, 0.0)
{
}

Colour::Colour(double r, double g, double b)
  : Vec3(r, g, b)
{
}

void Colour::toBytes(uint8_t& r, uint8_t& g, uint8_t& b) const
{
	r = static_cast<uint8_t>(((m_x < 0.0) ? 0.0 : ((m_x > 1.0) ? 1.0 : m_x)) * 255.0);
	g = static_cast<uint8_t>(((m_y < 0.0) ? 0.0 : ((m_y > 1.0) ? 1.0 : m_y)) * 255.0);
	b = static_cast<uint8_t>(((m_z < 0.0) ? 0.0 : ((m_z > 1.0) ? 1.0 : m_z)) * 255.0);
}
