#pragma once

#include "IDrawable.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Sprite : public IDrawable
{
public:

	Sprite();
	~Sprite() = default;

	sf::Sprite sprite;

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy(float deltaTime) override;
};

