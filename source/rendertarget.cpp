#include "rendertarget.h"

#include "colour.h"
#include <FreeImage.h>

RenderTarget::RenderTarget(unsigned width, unsigned height)
{
	m_bitmap = FreeImage_Allocate(width, height, 24);
}

RenderTarget::~RenderTarget()
{
	FreeImage_Unload(m_bitmap);
}

unsigned RenderTarget::getWidth() const
{
	return FreeImage_GetWidth(m_bitmap);
}

unsigned RenderTarget::getHeight() const
{
	return FreeImage_GetHeight(m_bitmap);
}

void RenderTarget::setPixel(unsigned x, unsigned y, const Colour& colour)
{
	RGBQUAD rgb;
	rgb.rgbRed = colour.r;
	rgb.rgbGreen = colour.g;
	rgb.rgbBlue = colour.b;
	FreeImage_SetPixelColor(m_bitmap, x, getHeight() - 1 - y, &rgb);
}

void RenderTarget::save(const char* filename)
{
	FreeImage_AdjustColors(m_bitmap, 0.0, 0.0, 2.2);
	FreeImage_Save(FIF_PNG, m_bitmap, filename);
}
