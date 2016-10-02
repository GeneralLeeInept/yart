#pragma once

#include "vec3f.h"

#include <memory>

class RayDifferentials;
typedef std::shared_ptr<class Texture> TexturePtr;

class Material
{
public:
	Vec3f Ka;
	Vec3f Kd;
	Vec3f Ks;
	float Ns;

	TexturePtr map_Kd;

	void shade(const Vec3f& V, const Vec3f& P, const Vec3f& N, const Vec3f& ST, float u, float v,
	           Vec3f& colour, RayDifferentials& rd) const;
};