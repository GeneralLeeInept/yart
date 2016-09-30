#pragma once

#include "texturemanager.h"
#include <map>
#include <memory>
#include <string>

class Camera;
class Mesh;
class RenderTarget;
class SceneObject;
struct __RTCDevice;
struct __RTCScene;

typedef std::shared_ptr<class Texture> TexturePtr;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void addMesh(Mesh* mesh);
	void commitScene();
	TexturePtr loadTexture(const std::string& filename);
	void render(const Camera& camera, RenderTarget& target);

private:
	typedef std::map<unsigned, std::unique_ptr<SceneObject>> ObjectMap;

	__RTCDevice* m_device;
	__RTCScene* m_scene;
	bool m_needsCommit;
	ObjectMap m_geometry;
	TextureManager m_textureManager;
};