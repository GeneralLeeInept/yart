#pragma once

#include "vec3f.h"

struct FIBITMAP;

class Texture
{
public:
	Texture(FIBITMAP* bitmap);
	~Texture();

	Vec3f at(float u, float v) const;

private:
	FIBITMAP* m_bitmap;
};