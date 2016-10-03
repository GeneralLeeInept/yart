#pragma once

#include <unordered_map>
#include <memory>
#include <string>

class Mesh;
class Scene;
class RenderTarget;
struct __RTCDevice;
struct __RTCScene;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void setScene(Scene* scene);
	void render(RenderTarget& target);

private:
	__RTCDevice* m_device;
	__RTCScene* m_rtcScene;
	std::unordered_map<unsigned, Mesh*> m_geometry;
	Scene* m_scene;
};