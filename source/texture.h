#pragma once

#include "vec3f.h"
#include <vector>

struct FIBITMAP;

class Texture
{
public:
	Texture(FIBITMAP* bitmap);
	~Texture();

	Vec3f at(float u, float v, unsigned lod) const;

private:
	std::vector<FIBITMAP*> m_mips;

	void buildMipChain(FIBITMAP* mip0);
};