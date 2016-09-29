#pragma once

#include <map>

class Camera;
class Mesh;
class RenderTarget;
struct __RTCDevice;
struct __RTCScene;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void addMesh(const Mesh& mesh);
	void commitScene();
	void render(const Camera& camera, RenderTarget& target);

private:
	typedef std::map<unsigned, const Mesh*> GeometryMap;

	__RTCDevice* m_device;
	__RTCScene* m_scene;
	GeometryMap m_geometry;
};