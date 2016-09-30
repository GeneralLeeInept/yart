#include "texture.h"

#include "math.h"
#include <FreeImage.h>

Texture::Texture(FIBITMAP* bitmap)
  : m_bitmap(bitmap)
{
}

Texture::~Texture()
{
	FreeImage_Unload(m_bitmap);
}

Vec3f Texture::at(float u, float v) const
{
	u = fmodf(u, 1.0f);
	if (u < 0.0f) u += 1.0f;
	v = fmodf(v, 1.0f);
	if (v < 0.0f) v += 1.0f;
	int x = static_cast<int>(u * FreeImage_GetWidth(m_bitmap));
	int y = static_cast<int>(v * FreeImage_GetHeight(m_bitmap));
	RGBQUAD pixel;
	FreeImage_GetPixelColor(m_bitmap, x, y, &pixel);
	return Vec3f(pixel.rgbRed / 255.0f, pixel.rgbGreen / 255.0f, pixel.rgbBlue / 255.0f);
}
