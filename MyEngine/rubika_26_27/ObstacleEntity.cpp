#include "ObstacleEntity.h"

#include "RenderComponent.h"
#include "RectangleShape.h"
#include "DynamicColliderComponent.h"
#include "Globals.h"
#include "TextureMgr.h"

ObstacleEntity::ObstacleEntity(std::string name) : Entity(name)
{
    TextureData defaultTexture = gData.TextureMgr->GetDefaultTextureData();

    RenderComponent* rc = this->AddComponent<RenderComponent>();
    {
        RectangleShape* rect = rc->AddNewDrawable<RectangleShape>("Rect", sf::Vector2f(), 0, sf::Vector2f(1, 1));
        
        rect->rectangleShape.setSize(sf::Vector2f(50, 50));
        rect->rectangleShape.setFillColor(sf::Color::White);
        
        rect->SetVisibility(true);
        rect->SetTexture("default");
    }

    DynamicColliderComponent* dcc = this->AddComponent<DynamicColliderComponent>();
    {
        dcc->SetHitboxSize(sf::Vector2f(50, 50));
        dcc->SetHitboxOffset(sf::Vector2f(0, 0));
    }
}

ObstacleEntity::~ObstacleEntity()
{
}
