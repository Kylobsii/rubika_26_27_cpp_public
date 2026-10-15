#include "TransformComponent.h"
#include "Entity.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

TransformComponent::TransformComponent(Entity& entity) : IComponent(entity)
{
	WorldPosition = sf::Vector2f();
	WorldPositionQuery = sf::Vector2f();
	Rotation = 0.f;
	Scale = sf::Vector2f(1.f, 1.f);

	UpdateMatrix();
}

const sf::Transform& TransformComponent::GetMatrix() const
{
	return Matrix;
}

void TransformComponent::SetWorldPosition(sf::Vector2f newPosition)
{
	WorldPosition = newPosition;
	WorldPositionQuery = newPosition;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetWorldPosition() const
{
	return WorldPosition;
}

void TransformComponent::SetWorldPositionQuery(sf::Vector2f newPosition)
{
	WorldPositionQuery = newPosition;
}

sf::Vector2f TransformComponent::GetWorldPositionQuery() const
{
	return WorldPositionQuery;
}

void TransformComponent::SetRotation(float newRotation)
{
	Rotation = newRotation;
	UpdateMatrix();
}

float TransformComponent::GetRotation() const
{
	return Rotation;
}

void TransformComponent::SetScale(sf::Vector2f newScale)
{
	Scale = newScale;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetScale() const
{
	return Scale;
}

void TransformComponent::Start()
{}

void TransformComponent::Update(float fDeltaTime)
{}

void TransformComponent::Destroy()
{}

void TransformComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::Text("Position: (%.2f, %.2f)", GetWorldPosition().x, GetWorldPosition().y);
	ImGui::Text("Rotation: %.2f degrees", GetRotation());
	ImGui::Text("Scale: (%.2f, %.2f)", GetScale().x, GetScale().y);
#endif
}

std::string TransformComponent::GetName()
{
	return "Transform";
}

void TransformComponent::UpdateMatrix()
{
	Matrix = sf::Transform::Identity;
	Matrix.translate(WorldPosition);
	Matrix.rotate(sf::degrees(Rotation));
	Matrix.scale(Scale);
}
