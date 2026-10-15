#pragma once

#include "IDrawable.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class RectangleShape : public IDrawable
{
public:

	RectangleShape();
	~RectangleShape() = default;

	sf::RectangleShape rectangleShape;

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy(float deltaTime) override;
};

