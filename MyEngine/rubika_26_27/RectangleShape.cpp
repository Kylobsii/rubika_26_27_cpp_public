#include "RectangleShape.h"

#include "Logger.h"
#include "Globals.h"

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

void RectangleShape::Destroy()
{
}

std::string RectangleShape::GetName()
{
	return "Rect";
}

bool RectangleShape::SetTexture(const std::string& textureName)
{
	textureData = gData.TextureMgr->GetTextureData(textureName);

	this->textureName = textureName;
	rectangleShape.setTexture(&textureData.Texture);

	return true;
}

bool RectangleShape::SetStaticTile(const std::string& staticTileName)
{
	for (const auto& [key, val] : textureData.StaticTilesData)
	{
		if (key == staticTileName)
		{
			rectangleShape.setOrigin(sf::Vector2f(val.StartX, val.StartY));
			rectangleShape.setSize(sf::Vector2f(val.SizeX, val.SizeY));
			rectangleShape.setScale(sf::Vector2f(val.IsRevertedX ? -1 : 1, val.IsRevertedY ? -1 : 1));
		
			return true;
		}
	}

	Logger::Warning("No static tile data of " + textureName + " matches the name: " + staticTileName);

	return false;
}

bool RectangleShape::SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName)
{
	bool result = SetTexture(textureName);
	if (result) { result = SetStaticTile(staticTileName); }

	return result;
}
