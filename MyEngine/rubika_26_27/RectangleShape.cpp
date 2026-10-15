#include "RectangleShape.h"

RectangleShape::RectangleShape()
{
	Drawable = &rectangleShape;
}

void RectangleShape::Start()
{
	rectangleShape.setFillColor(sf::Color());
}

void RectangleShape::Update(float deltaTime)
{
}

void RectangleShape::Destroy(float deltaTime)
{
}