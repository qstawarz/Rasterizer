#include <iostream>

#include "../../includes/World/Scene.h"

using namespace World;

Scene::Scene()
{
//	std::cout << "Scene created" << std::endl;
}

Scene::~Scene()
{
	for (size_t i = 0; i < m_entities.size(); ++i)
		delete m_entities[i].GetMesh();

//	std::cout << "Scene detroyed" << std::endl;
}

void Scene::AddEntity(Entity&& p_entity)
{
	m_entities.push_back(p_entity);
}

void Scene::AddLight(Light&& p_light)
{
	m_lights.push_back(p_light);
}

void Scene::AddCamera(Camera&& p_camera)
{
	m_cameras.push_back(p_camera);
}

const std::vector<Entity>& Scene::GetEntities() const { return m_entities; }
const std::vector<Light>& Scene::GetLights() const { return m_lights; }
const std::vector<Camera>& Scene::GetCameras() const { return m_cameras; }
