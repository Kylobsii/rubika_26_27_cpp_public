#pragma once

#include "IDrawable.h"
#include "TextureMgr.h"

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
	virtual void Destroy() override;

	virtual std::string GetName() override;

	void Reset();

	void SetTexture(const std::string& textureName);
	void SetAnimation(const std::string& animationName);

	void EnableAnimation(bool play);

protected:

	AnimationData CurrentAnimationData;

	std::string CurrentTexture;
	std::string CurrentAnimation;

	int CurrentAnimationNb = 0;
	float CurrentAnimationTime = 0.f;

	bool PlayAnimation = true;
};

