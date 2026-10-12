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

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
};