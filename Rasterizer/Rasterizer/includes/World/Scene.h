#pragma once

#include <vector>

#include "Entity.h"
#include "Light.h"
#include "Camera.h"

namespace World
{
	class Scene
	{
	private:
		std::vector<Entity> m_entities;
		std::vector<Light> m_lights;
		std::vector<Camera> m_cameras;

	public:
		Scene();
		~Scene();

		void AddEntity(Entity&& p_entity);
		void AddLight(Light&& p_light);
		void AddCamera(Camera&& p_camera);

		const std::vector<Entity>& GetEntities() const;
		const std::vector<Light>& GetLights() const;
		const std::vector<Camera>& GetCameras() const;
	};
}
