#include "Entity.h"
#include "Scene.h"

Scene::Scene(const std::string& name)
{
	this->name = name;
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

void Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}

void Scene::RemoveEntity(Entity* entity)
{
	std::erase(entities, entity);
}