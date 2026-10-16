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

	WorldPosXBuffer[0] = '\0';
	WorldPosYBuffer[0] = '\0';

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

	float position[2] = { GetWorldPosition().x, GetWorldPosition().y };
	std::string field_id = "##pos##" + GetEntity().name + "##" + GetName();
	ImGui::Text("Position:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputFloat2(field_id.c_str(), position, "%.2f");
	SetWorldPosition(sf::Vector2f(position[0], position[1]));

	float rotation = GetRotation();
	field_id = "##rotation##" + GetEntity().name + "##" + GetName();
	ImGui::Text("Rotation:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputFloat(field_id.c_str(), &rotation, 0.0f, 0.0f, "%.2f");
	SetRotation(rotation);

	float scale[2] = { GetScale().x, GetScale().y };
	field_id = "##scale##" + GetEntity().name + "##" + GetName();
	ImGui::Text("Scale:   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputFloat2(field_id.c_str(), scale, "%.2f");
	SetScale(sf::Vector2f(scale[0], scale[1]));
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
