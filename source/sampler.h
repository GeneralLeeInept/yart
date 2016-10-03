#pragma once

#include "vec3f.h"
#include <memory>

class RayDifferentials;
class Texture;

class Sampler
{
public:
	void bind(Texture* texture);
	Vec3f operator()(const Vec3f& texcoord, const RayDifferentials& rd) const;

private:
	Texture* m_texture;

	Vec3f wrap(const Vec3f& texcoord) const;
};