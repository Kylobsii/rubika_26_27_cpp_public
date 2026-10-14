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

void Sprite::Destroy(float deltaTime)
{
}
