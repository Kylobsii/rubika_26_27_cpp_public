#pragma once

#include <vector>
#include <string>

namespace sf
{
	class RenderWindow;
}

class Entity;

class Scene
{
public:

	Scene(const std::string& name);
	~Scene();

	std::string name;

	std::vector<Entity*> entities;

	int GetEntityID();

	char entityNameBuffer[128];
	std::string newEntityName;

	void AddEntity(std::string name);
	void RemoveEntity(Entity* entity);

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

private:
	int createdEntityCount = 0;
};