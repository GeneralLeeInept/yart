#include "sampler.h"

#include "math.h"
#include "raydifferentials.h"
#include "texture.h"

void Sampler::bind(const TexturePtr& texture)
{
	m_texture = texture;
}

Vec3f Sampler::operator()(const Vec3f& texcoord, const RayDifferentials& rd) const
{
	if (m_texture)
	{
		Vec3f wrapped = wrap(texcoord);
		unsigned lod = 1;
		return m_texture->at(wrapped.x, wrapped.y, lod);
	}
	else
	{
		return Vec3f::Zero;
	}
}

Vec3f Sampler::wrap(const Vec3f& texcoord) const
{
	Vec3f o;
	for (int i = 0; i < 3; ++i)
	{
		o[i] = fmodf(texcoord[i], 1.0f);
		if (o[i] < 0.0f)
			o[i] += 1.0f;
	}
	return o;
}
