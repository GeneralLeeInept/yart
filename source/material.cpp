#include "material.h"

#include "light.h"
#include "sampler.h"
#include "texture.h"
#include <algorithm>
#include <embree2/rtcore_ray.h>

Vec3f Material::shade(const Vec3f& V, const Vec3f& P, const Vec3f& N, const RTCRay& ray, const Light& light,
                      const Vec3f& ST, RayDifferentials& rd) const
{
	Vec3f Cd;

	if (map_Kd)
	{
		Sampler sampler;
		sampler.bind(map_Kd);
		Cd = sampler(ST, rd);
	}
	else
	{
		Cd = Kd;
	}

	Vec3f L = light.L(P);
	Vec3f H = V + L;
	H.normalise();

	float Sd = std::min(1.0f, std::max(0.0f, Vec3f::dot(N, L))) * light.intensity(P);
	float Ss = std::pow(std::min(1.0f, std::max(0.0f, Vec3f::dot(N, H))), Ns) * light.intensity(P);
	return Cd * (light.m_colour * Sd) + Ks * (light.m_colour * Ss);
}
