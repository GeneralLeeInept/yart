#include "material.h"

#include "sampler.h"
#include "texture.h"
#include <algorithm>

void Material::shade(const Vec3f& V, const Vec3f& P, const Vec3f& N, const Vec3f& ST, float u, float v,
                     Vec3f& colour, RayDifferentials& rd) const
{
	Vec3f diffuse;

	if (map_Kd)
	{
		Sampler sampler;
		sampler.bind(map_Kd);
		diffuse = sampler(ST, rd);
	}
	else
	{
		diffuse = Kd;
	}

	Vec3f L(1.0f, 5.0f, -1.0f);
	L.normalise();

	Vec3f H = V + L;
	H.normalise();

	colour = /*KaIa +*/
	  diffuse * std::min(1.0f, std::max(0.0f, Vec3f::dot(N, L))) //{ SUM j = 1..ls, (N*Lj)Ij }
	  + Ks * std::pow(std::min(1.0f, std::max(0.0f, Vec3f::dot(N, H))), Ns); //{ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij }
}
