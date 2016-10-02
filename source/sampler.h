#pragma once

#include "vec3f.h"
#include <memory>

class RayDifferentials;
typedef std::shared_ptr<class Texture> TexturePtr;

class Sampler
{
public:
	void bind(const TexturePtr& texture);
	Vec3f operator()(const Vec3f& texcoord, const RayDifferentials& rd) const;

private:
	TexturePtr m_texture;

	Vec3f wrap(const Vec3f& texcoord) const;
};