#pragma once

#include "shadable.h"

class Renderer;

class SceneObject : public IShadable
{
public:
	SceneObject(Renderer& renderer);

protected:
	Renderer& m_renderer;
};
