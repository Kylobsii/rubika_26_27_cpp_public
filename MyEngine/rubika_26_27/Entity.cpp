#include "Entity.h"

#include "Sprite.h"
#include "Globals.h"
#include "TextureMgr.h"

#ifdef _DEBUG
#include "DynamicColliderComponent.h"
#endif

Entity::Entity(std::string friendlyName) : name(friendlyName)
{
}

Entity::~Entity()
{
	Destroy();
}

void Entity::Start()
{
	for (auto component : components)
	{
		component->Start();
	}
}

void Entity::Update(float fDeltaTime)
{
	for (auto component : components)
	{
		component->Update(fDeltaTime);
	}
}

void Entity::Destroy()
{
	for (auto component : components)
	{
		component->Destroy();
		delete component;
	}
}

void Entity::Draw(sf::RenderWindow& window) const
{
	for (auto component : components)
	{
		if (RenderComponent* rend = dynamic_cast<RenderComponent*>(component))
		{
			rend->Draw(window);
		}

#ifdef _DEBUG
		if (DynamicColliderComponent* coll = dynamic_cast<DynamicColliderComponent*>(component))
		{
			coll->Draw(window);
		}
#endif
	}
}