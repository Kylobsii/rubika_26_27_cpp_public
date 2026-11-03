#include "DynamicColliderComponent.h"

#include "Entity.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif


DynamicColliderComponent::DynamicColliderComponent(Entity& entity) : IComponent(entity)
{
	tr = GetEntity().GetComponent<TransformComponent>();

	debugBoxDisplay.setOutlineColor(sf::Color::Green);
}

DynamicColliderComponent::~DynamicColliderComponent()
{
	Destroy();
}

void DynamicColliderComponent::Start()
{
}

void DynamicColliderComponent::Update(float fDeltaTime)
{
#ifdef _DEBUG
	sf::Vector2f scale = tr->GetScale();
	box.size = sf::Vector2f(boxData.LocalSize.x * scale.x, boxData.LocalSize.y * scale.y);

	sf::Vector2f worldPos = tr->GetWorldPosition();
	box.position = sf::Vector2f(boxData.LocalSize.x + worldPos.x, boxData.LocalSize.y + worldPos.y);
#endif
}

void DynamicColliderComponent::Destroy()
{
}


void DynamicColliderComponent::Draw(sf::RenderWindow& window) const
{
#ifdef _DEBUG
	sf::RenderStates states = sf::RenderStates::Default;

	sf::Transform boxTr = sf::Transform::Identity;
	boxTr.translate(box.position);

	states.transform = boxTr;

	window.draw(debugBoxDisplay, states);
#endif
}

void DynamicColliderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	float localSize[2] = { boxData.LocalSize.x, boxData.LocalSize.y };
	std::string field_id = "##scale##" + GetEntity().name + "##" + GetName();
	ImGui::Text("Local Size:   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputFloat2(field_id.c_str(), localSize, "%.2f");
	SetHitboxSize(sf::Vector2f(localSize[0], localSize[1]));

	float localPos[2] = { boxData.LocalPosition.x, boxData.LocalPosition.y };
	field_id = "##pos##" + GetEntity().name + "##" + GetName();
	ImGui::Text("LocalPosition:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputFloat2(field_id.c_str(), localPos, "%.2f");
	SetHitboxOffset(sf::Vector2f(localPos[0], localPos[1]));
#endif
}

std::string DynamicColliderComponent::GetName()
{
	return "Box Collision";
}

void DynamicColliderComponent::SetHitboxSize(sf::Vector2f size)
{
	boxData.LocalSize = size;

	sf::Vector2f scale = tr->GetScale();
	box.size = sf::Vector2f(size.x * scale.x, size.y * scale.y);

#ifdef _DEBUG
	debugBoxDisplay.setSize(box.size);
#endif
}

void DynamicColliderComponent::SetHitboxOffset(sf::Vector2f offset)
{
	boxData.LocalPosition = offset;

	sf::Vector2f worldPos = tr->GetWorldPosition();
	box.position = sf::Vector2f(offset.x + worldPos.x, offset.y + worldPos.y);
}

void DynamicColliderComponent::EnableCollision(bool bEnable)
{
	boxData.collisionChoice = bEnable ? OnCollisionChoice::Block : OnCollisionChoice::Ignore;
}

bool DynamicColliderComponent::AreCollisionsEnabled() const
{
	return boxData.collisionChoice == OnCollisionChoice::Block;
}

const sf::FloatRect& DynamicColliderComponent::GetHitbox() const
{
	return box;
}

void DynamicColliderComponent::SetOnCollisionCallback(std::function<OnCollisionChoice(const Entity*)> callback)
{
	Callbacks.push_back(callback);
}
