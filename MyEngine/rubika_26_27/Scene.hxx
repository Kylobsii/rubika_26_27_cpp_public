#include "Scene.h"
#include "Tags.h"
#include "Entity.h"

template <typename E>
requires std::is_base_of_v<Entity, E>
E* Scene::AddEntity(std::string name)
{
	E* entity = new E(name);

	entities.push_back(entity);

	if (entity->tags.HasAllTags(ENTITY_TAG_DYNAMIC))
	{
		dynamicEntities.push_back(entity);
	}
	else
	{
		staticEntities.push_back(entity);
	}

	return entity;
}