#include "RenderComponent.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

#include "Logger.h"

RenderComponent::RenderComponent(Entity& entity)
	: IComponent(entity) { }

void RenderComponent::Start()
{
	for (auto drawable : drawables)
	{
		drawable->Start();
	}
}

void RenderComponent::Update(float fDeltaTime)
{
	for (auto drawable : drawables)
	{
		drawable->Update(fDeltaTime);
	}
}

void RenderComponent::Destroy()
{
	for (auto drawable : drawables)
	{
		drawable->Destroy();
		delete drawable;
	}
}

void RenderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	int drCount = 0;
	for (IDrawable* dr : drawables)
	{
		ImGui::Text(" - %s", dr->GetName().c_str());
		//ImGui::Image(dr)

		drCount++;
	}
#endif
}

std::string RenderComponent::GetName()
{
	return "Renderer";
}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	for (auto drawable : drawables)
	{
		drawable->Draw(window);
	}
}
