#pragma once

#include "vec3f.h"

#include <memory>

class Light;
class RayDifferentials;
struct RTCRay;
class Texture;

class Material
{
public:
	Vec3f Ka;
	Vec3f Kd;
	Vec3f Ks;
	float Ns;

	Texture* map_Kd;

	Material();

	Vec3f shade(const Vec3f& V, const Vec3f& P, const Vec3f& N, const RTCRay& ray, const Light& light,
	           const Vec3f& ST, RayDifferentials& rd) const;
};