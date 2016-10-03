#pragma once

#include "vec3f.h"

#include <memory>

class RayDifferentials;
class Texture;

class Material
{
public:
	Vec3f Ka;
	Vec3f Kd;
	Vec3f Ks;
	float Ns;

	Texture* map_Kd;

	void shade(const Vec3f& V, const Vec3f& P, const Vec3f& N, const Vec3f& ST, float u, float v,
	           Vec3f& colour, RayDifferentials& rd) const;
};