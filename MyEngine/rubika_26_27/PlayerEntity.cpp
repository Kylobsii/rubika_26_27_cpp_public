#include "PlayerEntity.h"

#include "RenderComponent.h"
#include "Sprite.h"
#include "DynamicColliderComponent.h"
#include "Globals.h"
#include "InputMgr.h"

PlayerEntity::PlayerEntity(std::string name) : Entity(name)
{
    const char* texturePath = "../Ressources/IsaacSprite.png";
    const char* textureName = "IsaacSprite";
    gData.TextureMgr->LoadTexture(texturePath);

    RenderComponent* rc = this->AddComponent<RenderComponent>();
    {
        Sprite* bodySprite = rc->AddNewDrawable<Sprite>("Body", sf::Vector2f(2, 0), 0, sf::Vector2f(1, 1));
        bodySprite->SetVisibility(true);
        bodySprite->SetTexture(textureName);
        bodySprite->SetAnimation("Body_Vertical");

        Sprite* headSprite = rc->AddNewDrawable<Sprite>("Head", sf::Vector2f(-2.5f, -20), 0, sf::Vector2f(1, 1));
        headSprite->SetVisibility(true);
        headSprite->SetTexture(textureName);
        headSprite->SetAnimation("Head_Down");
    }

    DynamicColliderComponent* dcc = this->AddComponent<DynamicColliderComponent>();
    {
        dcc->SetHitboxSize(sf::Vector2f(25, 30));
        dcc->SetHitboxOffset(sf::Vector2f(-1, -18));

        auto collisionCallback = [](const Entity*)
            {
                return OnCollisionChoice::Block;
            };
        dcc->SetOnCollisionCallback(collisionCallback);
    }

    SetupBindings();
}

PlayerEntity::~PlayerEntity()
{
    DestroyBindings();
}

void PlayerEntity::SetupBindings()
{
    auto moveUpCallback = [this](float value)
        {
            GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(0, -1), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveUp", "Up Movement", moveUpCallback, this);

    auto moveDownCallback = [this](float value)
        {
            GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(0, 1), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveDown", "Down Movement", moveDownCallback, this);

    auto moveLeftCallback = [this](float value)
        {
            GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(-1, 0), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveLeft", "Left Movement", moveLeftCallback, this);

    auto moveRightCallback = [this](float value)
        {
            GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(1, 0), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveRight", "Right Movement", moveRightCallback, this);
}

void PlayerEntity::DestroyBindings()
{
    gData.InputMgr->UnregisterInputActionCallbacks("MoveUp", this);
    gData.InputMgr->UnregisterInputActionCallbacks("MoveDown", this);
    gData.InputMgr->UnregisterInputActionCallbacks("MoveLeft", this);
    gData.InputMgr->UnregisterInputActionCallbacks("MoveRight", this);
}
