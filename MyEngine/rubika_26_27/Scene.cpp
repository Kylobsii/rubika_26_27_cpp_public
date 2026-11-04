#include "Entity.h"
#include "Scene.h"
#include "DynamicColliderComponent.h"

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
			for (int j = i + 1; j < collidersInScene.size(); j++)
			{
				box2 = collidersInScene[j];
				if (overlap = box1->GetHitbox().findIntersection(box2->GetHitbox()))
				{
					box1->ExecuteCallbacks(&box2->GetEntity(), overlap.value());
					box2->ExecuteCallbacks(&box1->GetEntity(), overlap.value());
				}
			}
		}
	}
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