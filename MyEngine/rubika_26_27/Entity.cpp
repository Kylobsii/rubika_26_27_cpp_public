#include "Entity.h"

Entity::Entity(std::string friendlyName) 
	: name(friendlyName), transform(*this), renderer(*this) {}

Entity::~Entity()
{
	Destroy();
}

void Entity::Start()
{
}

void Entity::Update(float fDeltaTime)
{
}

void Entity::Destroy()
{
	transform.Destroy();
}