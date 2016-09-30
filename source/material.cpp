#include "material.h"

#include "texture.h"
#include <algorithm>

void Material::shade(const Vec3f& P, const Vec3f& N, const Vec3f& ST, float u, float v, Vec3f& colour) const
{
	Vec3f Lp(0.0f, 1000.0f, 0.0f);
	Vec3f L = Lp - P;
	L.normalise();

	Vec3f diffuse = map_Kd ? map_Kd->at(ST.x, ST.y) : Kd;

	colour = /*KaIa +*/
	  diffuse * std::min(1.0f, std::max(0.0f, Vec3f::dot(N, L))); //{ SUM j = 1..ls, (N*Lj)Ij }
	//+ Ks	//{ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij }
}
