#include "colour.h"

#include "vec3.h"

Colour::Colour()
  : r(0)
  , g(0)
  , b(0)
{
}

Colour::Colour(const Vec3& v)
{
	for (int i = 0; i < 3; ++i)
		rgb[i] = static_cast<uint8_t>((v.xyz[i] > 1.0f ? 1.0f : v.xyz[i]) * 255.0f);
}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
  : r(r)
  , g(g)
  , b(b)
{
}
