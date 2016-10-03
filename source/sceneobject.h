#pragma once

#include "shadable.h"

class Renderer;

class SceneObject : public IShadable
{
public:
	virtual ~SceneObject() = default;
};
