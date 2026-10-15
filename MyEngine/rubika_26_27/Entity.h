#pragma once

#include <string>
#include <vector>
#include <type_traits>
#include <typeinfo>

#include "IComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Logger.h"


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
	std::vector<IComponent*> components;

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	template <typename C>
	requires std::is_base_of_v<IComponent, C>
	C* AddComponent();

	template <typename C>
	requires std::is_base_of_v<IComponent, C>
	C* GetComponent(bool log = true) const;

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
};

#include "Entity.hxx"


