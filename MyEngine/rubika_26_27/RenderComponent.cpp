#include "RenderComponent.h"

RenderComponent::RenderComponent(Entity& entity)
	: IComponent(entity) { }

void RenderComponent::Start()
{
	rect.Start();
}

void RenderComponent::Update(float fDeltaTime)
{
}

void RenderComponent::Destroy()
{
}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	rect.Draw(window);
}
