#pragma once

#include "IComponent.h"
#include "RectangleShape.h"
#include "Logger.h"

#include <SFML/Graphics/Drawable.hpp>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <vector>


class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& entity);
	~RenderComponent() = default;

	std::vector<IDrawable*> drawables;

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;

	virtual void DrawDebug() override;
	virtual std::string GetName() override;

	template <typename D>
	requires std::is_base_of_v<IDrawable, D>
	D* AddDrawable();

	void Draw(sf::RenderWindow& window) const;
};

#include "RenderComponent.hxx"


