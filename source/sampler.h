#pragma once

#include "vec3f.h"
#include <memory>

typedef std::shared_ptr<class Texture> TexturePtr;

class Sampler
{
public:
	void bind(const TexturePtr& texture);
	Vec3f operator()(const Vec3f& texcoord) const;

private:
	TexturePtr m_texture;

	Vec3f wrap(const Vec3f& texcoord) const;
};