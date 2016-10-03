#pragma once

#include "camera.h"
#include "light.h"
#include "matrix33.h"
#include <memory>
#include <vector>

class Light;
class MeshCache;
class SceneObject;
class TextureCache;

class Scene
{
public:
	void addObject(SceneObject* object);
	void addLight(Light* light);
	void addCamera(Camera* camera);
	
	static Scene* load(const std::string& filename, MeshCache& meshCache, TextureCache& textureCache);

	struct SceneNode
	{
		SceneObject* object;
		Matrix33 orientation;
	};

	std::vector<std::unique_ptr<SceneNode>> m_objects;
	std::vector<std::unique_ptr<Light>> m_lights;
	std::unique_ptr<Camera> m_camera;
};