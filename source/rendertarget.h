#pragma once

#include <string>

struct FIBITMAP;
class Colour;

class RenderTarget
{
public:
	RenderTarget(unsigned width, unsigned height);
	~RenderTarget();

	unsigned getWidth() const;
	unsigned getHeight() const;

	void setPixel(unsigned x, unsigned y, const Colour& colour);

	void save(const std::string& filename);

private:
	FIBITMAP* m_bitmap;
};