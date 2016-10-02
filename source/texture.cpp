#include "texture.h"

#include <FreeImage.h>

Texture::Texture(FIBITMAP* bitmap)
{
	buildMipChain(bitmap);
}

Texture::~Texture()
{
	for (FIBITMAP* bitmap : m_mips)
	{
		FreeImage_Unload(bitmap);
	}
}

Vec3f Texture::at(float u, float v, unsigned lod) const
{
	int x = static_cast<int>(u * FreeImage_GetWidth(m_mips[lod]));
	int y = static_cast<int>(v * FreeImage_GetHeight(m_mips[lod]));
	RGBQUAD pixel;
	FreeImage_GetPixelColor(m_mips[lod], x, y, &pixel);
	return Vec3f(pixel.rgbRed / 255.0f, pixel.rgbGreen / 255.0f, pixel.rgbBlue / 255.0f);
}

void Texture::buildMipChain(FIBITMAP* mip0)
{
	unsigned w = FreeImage_GetWidth(mip0);
	unsigned h = FreeImage_GetHeight(mip0);
	unsigned mips;

	m_mips.push_back(mip0);

	for (mips = 0; w > 0 || h > 0; w >>= 1, h >>= 1, mips++)
	{
	}

	w = FreeImage_GetWidth(mip0);
	h = FreeImage_GetHeight(mip0);

	for (unsigned mip = 0; mip < mips; ++mip)
	{
		w = (w > 1) ? (w >> 1) : w;
		h = (h > 1) ? (h >> 1) : h;
		m_mips.push_back(FreeImage_Rescale(mip0, w, h, FILTER_BICUBIC));
	}
}
