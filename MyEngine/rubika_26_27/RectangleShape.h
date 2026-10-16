#pragma once

#include "IDrawable.h"
#include "TextureMgr.h"

#include <string>
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

	std::string textureName = "Empty";
	TextureData textureData;

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual std::string GetName() override;

	bool SetTexture(const std::string& textureName);
	bool SetStaticTile(const std::string& staticTileName);
	bool SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName);
};

