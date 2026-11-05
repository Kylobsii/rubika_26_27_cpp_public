#include "DynamicColliderComponent.h"

#include "Entity.h"
#include "Globals.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif


DynamicColliderComponent::DynamicColliderComponent(Entity& entity) : IComponent(entity)
{
	tr = GetEntity().GetComponent<TransformComponent>();

	debugBoxDisplay.setFillColor(sf::Color::Transparent);
	debugBoxDisplay.setOutlineColor(sf::Color::Green);
	debugBoxDisplay.setOutlineThickness(-2.f);
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
	sf::Vector2f scale = tr->GetScale();
	box.size = sf::Vector2f(boxData.LocalSize.x * scale.x, boxData.LocalSize.y * scale.y);

	sf::Vector2f worldPos = tr->GetWorldPosition();
	box.position = sf::Vector2f(boxData.LocalPosition.x + worldPos.x, boxData.LocalPosition.y + worldPos.y);

#ifdef _DEBUG
	debugBoxDisplay.setSize(box.size);
#endif
}

void DynamicColliderComponent::Destroy()
{
}


void DynamicColliderComponent::Draw(sf::RenderWindow& window) const
{
#ifdef _DEBUG
	if (gData.DebugMgr->drawColliders)
	{
		sf::RenderStates states = sf::RenderStates::Default;

		sf::Transform boxTr = sf::Transform::Identity;
		boxTr.translate(box.position);

		states.transform = boxTr;

		window.draw(debugBoxDisplay, states);
	}
#endif
}

void DynamicColliderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	//float localSize[2] = { boxData.LocalSize.x, boxData.LocalSize.y };
	std::string field_id = "##scale##" + GetEntity().name + "##" + GetName();
	ImGui::Text("Local Size:   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputFloat2(field_id.c_str(), &boxData.LocalSize.x, "%.2f");
	//SetHitboxSize(sf::Vector2f(localSize[0], localSize[1]));

	//float localPos[2] = { boxData.LocalPosition.x, boxData.LocalPosition.y };
	field_id = "##pos##" + GetEntity().name + "##" + GetName();
	ImGui::Text("LocalPosition:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputFloat2(field_id.c_str(), &boxData.LocalPosition.x, "%.2f");
	//SetHitboxOffset(sf::Vector2f(localPos[0], localPos[1]));
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

void DynamicColliderComponent::ExecuteCallbacks(const Entity* other, sf::FloatRect overlapBox)
{
	for (auto callback : Callbacks)
	{
		if (callback(other) == OnCollisionChoice::Block)
		{
			sf::Vector2f dir;
			bool dirBool;

			if (overlapBox.size.x >= overlapBox.size.y)
			{
				dir = sf::Vector2f(0, overlapBox.size.y);
				dirBool = tr->GetWorldPosition().y > other->GetComponent<TransformComponent>()->GetWorldPosition().y;
				dir = dirBool ? dir : -dir;
			}
			else
			{
				dir = sf::Vector2f(overlapBox.size.x, 0);
				dirBool = tr->GetWorldPosition().x > other->GetComponent<TransformComponent>()->GetWorldPosition().x;
				dir = dirBool ? dir : -dir;
			}

			tr->SetWorldPosition(tr->GetWorldPosition() + dir);
		}
	}
}
