#include "Scene.h"

#include "Entity.h"
#include "DynamicColliderComponent.h"
#include "Profiler.h"

Scene::Scene(const std::string& name)
{
	this->name = name;
	entityNameBuffer[0] = '\0';
}

Scene::~Scene()
{
	Destroy();
}

void Scene::Start()
{
	for(int i = 0; i < entities.size(); i++)
	{
		entities[i]->Start();
	}
}

void Scene::Update(float fDeltaTime)
{
	PROFILER_EVENT_BEGIN(PROFILER_COLOR_PURPLE, "Collision Check");

	std::vector<DynamicColliderComponent*> collidersInScene;

	for(int i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(fDeltaTime);

		if (DynamicColliderComponent* colliderPtr = entities[i]->GetComponent<DynamicColliderComponent>())
		{
			collidersInScene.emplace_back(colliderPtr);
		}
	}

	DynamicColliderComponent* box1;
	DynamicColliderComponent* box2;
	std::optional<sf::FloatRect> overlap;
	if (collidersInScene.size() > 1)
	{
		for (int i = 0; i < collidersInScene.size() - 1; i++)
		{
			box1 = collidersInScene[i];

			if (!box1->CollisionsEnabled()) continue;

			for (int j = i + 1; j < collidersInScene.size(); j++)
			{
				box2 = collidersInScene[j];

				if (!box2->CollisionsEnabled()) continue;
				if (!box1->CanCollide(box2)) continue;

				if (overlap = box1->GetHitbox().findIntersection(box2->GetHitbox()))
				{
					box1->ExecuteCallbacks(&box2->GetEntity(), overlap.value());
					box2->ExecuteCallbacks(&box1->GetEntity(), overlap.value());
				}
			}
		}
	}

	PROFILER_EVENT_END();
}

void Scene::Destroy()
{
	for(int i = 0; i < entities.size(); i++)
	{
		entities[i]->Destroy();
	}
}

int Scene::GetEntityID()
{
	int result = createdEntityCount;
	createdEntityCount++;

	return result;
}

void Scene::RemoveEntity(Entity* entity)
{
	std::erase(entities, entity);
	delete entity;
}

void Scene::Draw(sf::RenderWindow& window) const
{
	for (Entity* entity : entities)
	{
		entity->Draw(window);
	}
}

void Scene::UpdateEntitySorting(Entity* entity, bool changingDynamicVec, bool removing)
{
	if (changingDynamicVec)
	{
		if (removing)
		{
			dynamicEntities.erase(std::remove(dynamicEntities.begin(), dynamicEntities.end(), entity), dynamicEntities.end());
		}
		else
		{
			dynamicEntities.push_back(entity);
		}
	}
	else
	{
		if (removing)
		{
			staticEntities.erase(std::remove(staticEntities.begin(), staticEntities.end(), entity), staticEntities.end());
		}
		else
		{
			staticEntities.push_back(entity);
		}
	}
}
