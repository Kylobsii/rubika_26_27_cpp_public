#include "Entity.h"
#include "Scene.h"

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
	for(int i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(fDeltaTime);
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

void Scene::AddEntity(std::string name)
{
	Entity* entity = new Entity(name);

	entities.push_back(entity);
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