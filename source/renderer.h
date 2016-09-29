#pragma once

#include <map>
#include <memory>

class Camera;
class Mesh;
class RenderTarget;
class SceneObject;
struct __RTCDevice;
struct __RTCScene;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void addMesh(Mesh* mesh);
	void commitScene();
	void render(const Camera& camera, RenderTarget& target);

private:
	typedef std::map<unsigned, std::unique_ptr<SceneObject>> ObjectMap;

	__RTCDevice* m_device;
	__RTCScene* m_scene;
	bool m_needsCommit;
	ObjectMap m_geometry;
};