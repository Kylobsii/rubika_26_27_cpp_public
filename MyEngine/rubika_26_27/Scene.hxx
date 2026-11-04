#include "Scene.h"

template <typename E>
requires std::is_base_of_v<Entity, E>
E* Scene::AddEntity(std::string name)
{
	E* entity = new E(name);

	entities.push_back(entity);
	return entity;
}