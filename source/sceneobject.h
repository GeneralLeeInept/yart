#pragma once

#include "shadable.h"

class Renderer;

class SceneObject : public IShadable
{
public:
	SceneObject(const Renderer& renderer);

protected:
	const Renderer& m_renderer;
};
