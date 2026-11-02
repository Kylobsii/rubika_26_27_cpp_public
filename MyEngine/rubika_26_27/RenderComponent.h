#pragma once

#include "IComponent.h"
#include "RectangleShape.h"
#include "Logger.h"

#include <SFML/Graphics/Drawable.hpp>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <vector>

class TransformComponent;

class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& entity);
	~RenderComponent() = default;

	//std::vector<IDrawable*> drawables;

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;

	virtual void DrawDebug() override;
	virtual std::string GetName() override;

	template <class D>
	D* AddNewDrawable(const std::string& drawableFriendlyName);

	template <class D>
	D* AddNewDrawable(const std::string& drawableFriendlyName, sf::Vector2f relativePos, float relativeAngle, sf::Vector2f relativeScale);

	void SetDrawableRelativePosition(const IDrawable* drawable, const sf::Vector2f& position);
	void SetDrawableRelativeRotation(const IDrawable* drawable, float rotation);
	void SetDrawableRelativeScale(const IDrawable* drawable, const sf::Vector2f& scale);

	void Draw(sf::RenderWindow& window) const;

protected:

	struct DrawableInfo
	{
		DrawableInfo();

		std::string FriendlyName;
		IDrawable* Drawable;
		sf::Vector2f RelativePosition;
		float RelativeRotation;
		sf::Vector2f RelativeScale;
		sf::Transform RelativeTransform;
		bool HasRelativeTransform;

		void ComputeTransform();
	};

	std::vector<DrawableInfo> Drawables;
	TransformComponent* Transform;
};

#include "RenderComponent.hxx"


