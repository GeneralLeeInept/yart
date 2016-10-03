#include "scene.h"

#include "filepath.h"
#include "mesh.h"
#include "meshcache.h"
#include <fstream>
#include <vector>

void Scene::addObject(SceneObject* object)
{
	std::unique_ptr<SceneNode> node = std::make_unique<SceneNode>();
	node->object = object;
	m_objects.push_back(std::move(node));
}

void Scene::addLight(Light* light)
{
	m_lights.push_back(std::unique_ptr<Light>(light));
}

void Scene::addCamera(Camera* camera)
{
	m_camera.reset(camera);
}

static void tokenize(const std::string& line, const char* control, std::vector<std::string>& tokens)
{
	size_t pos = line.find_first_not_of(control, 0);

	while (pos != std::string::npos)
	{
		size_t eot = line.find_first_of(control, pos);
		size_t tokenLength = (eot == std::string::npos) ? eot : (eot - pos);
		std::string token = line.substr(pos, tokenLength);
		tokens.push_back(token);
		pos = line.find_first_not_of(control, eot);
	}
}

void processDirectionalLight(std::ifstream& file, Scene* scene)
{
	std::string line;
	while (std::getline(file, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "}")
			break;

		// todo etc
	}
}

void processPointLight(std::ifstream& file, Scene* scene)
{
	std::string line;
	while (std::getline(file, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "}")
			break;

		// todo etc
	}
}

void processCamera(std::ifstream& file, Scene* scene)
{
	std::string line;
	Camera* camera = new Camera;

	while (std::getline(file, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "}")
			break;

		if (tokens[0] == "pos")
		{
			for (int i = 0; i < 3; ++i)
				camera->m_position[i] = static_cast<float>(atof(tokens[i + 1].c_str()));
		}

		if (tokens[0] == "rot")
		{
			camera->m_orientation.rotateY(static_cast<float>(atof(tokens[2].c_str())));
			camera->m_orientation.rotateX(static_cast<float>(atof(tokens[1].c_str())));
			// todo: Z rotation
		}
	}

	scene->addCamera(camera);
}

Scene* Scene::load(const std::string& filename, MeshCache& meshCache, TextureCache& textureCache)
{
	FilePath path(filename);
	std::ifstream file((const std::string&)path);
	Scene* scene = new Scene();

	std::string line;
	while (std::getline(file, line))
	{
		std::vector<std::string> tokens;
		tokenize(line, " \t\r\n", tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "mesh")
		{
			scene->addObject(meshCache.get(tokens[1], textureCache));
		}
		else if (tokens[0] == "directionallight")
		{
			processDirectionalLight(file, scene);
		}
		else if (tokens[0] == "pointlight")
		{
			processPointLight(file, scene);
		}
		else if (tokens[0] == "camera")
		{
			processCamera(file, scene);
		}
	}

	return scene;
}
