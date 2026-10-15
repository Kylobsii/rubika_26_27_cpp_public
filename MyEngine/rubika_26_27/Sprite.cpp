#include "Sprite.h"

#include "TextureMgr.h"

Sprite::Sprite() : sprite(TextureMgr::GetDefaultTextureData().Texture)
{
	Drawable = &sprite;
}

void Sprite::Start()
{
}

void Sprite::Update(float deltaTime)
{
}

void Sprite::Destroy()
{
}

std::string Sprite::GetName()
{
	return "Sprite";
}
