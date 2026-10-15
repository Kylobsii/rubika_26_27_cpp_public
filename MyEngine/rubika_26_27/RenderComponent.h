#pragma once

#include "IComponent.h"
#include "RectangleShape.h"

#include <SFML/Graphics/Drawable.hpp>


class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& entity);
	~RenderComponent() = default;

	RectangleShape rect;

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;

	void Draw(sf::RenderWindow& window) const;
};

