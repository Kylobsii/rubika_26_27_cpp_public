#pragma once

#include <string>
#include "TransformComponent.h"

namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	Entity(std::string friendlyName = "");
	virtual ~Entity();

	std::string name;
	TransformComponent transform;

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
};
