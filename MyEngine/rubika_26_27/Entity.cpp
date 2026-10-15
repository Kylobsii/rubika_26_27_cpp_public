#include "Entity.h"

Entity::Entity(std::string friendlyName) 
	: name(friendlyName), transform(*this), renderer(*this) {}

Entity::~Entity()
{
	Destroy();
}

void Entity::Start()
{
	renderer.Start();
}

void Entity::Update(float fDeltaTime)
{
}

void Entity::Destroy()
{
	transform.Destroy();
}

void Entity::Draw(sf::RenderWindow& window) const
{
	renderer.Draw(window);
}