#include "RenderComponent.h"

template <typename D>
requires std::is_base_of_v<IDrawable, D>
inline D* RenderComponent::AddDrawable()
{
	D* draw = new D();
	drawables.push_back(draw);

	return draw;
}