#pragma once

#include <IComponent.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <functional>
#include "TransformComponent.h"
#include "Tags.h"

enum class OnCollisionChoice
{
	Ignore,
	Block,
};

class DynamicColliderComponent : public IComponent
{
public:

	struct ColliderData
	{
		OnCollisionChoice collisionChoice = OnCollisionChoice::Block;

		Tags collisionMask;
		bool RequireAllFilterMode = false;

		sf::Vector2f LocalSize = sf::Vector2f(1, 1);
		sf::Vector2f LocalPosition = sf::Vector2f();
	};

	DynamicColliderComponent(Entity& entity);
	~DynamicColliderComponent();

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;

	void Draw(sf::RenderWindow& window) const;
	virtual void DrawDebug() override;
	virtual std::string GetName() override;

	void SetHitboxSize(sf::Vector2f size);
	void SetHitboxOffset(sf::Vector2f offset);
	void SetCollisionMask(int tags, bool requireAll = false);

	void EnableCollision(bool bEnable);
	bool CollisionsEnabled() const;
	bool CanCollide(DynamicColliderComponent* other);

	const sf::FloatRect& GetHitbox() const;

	void SetOnCollisionCallback(std::function<OnCollisionChoice(const Entity*)> callback);
	void ExecuteCallbacks(const Entity* other, sf::FloatRect overlapBox);

private:
	sf::FloatRect box;
	ColliderData boxData;

	TransformComponent* tr;

	std::vector< std::function<OnCollisionChoice(const Entity*)>> Callbacks;

	sf::RectangleShape debugBoxDisplay;
};