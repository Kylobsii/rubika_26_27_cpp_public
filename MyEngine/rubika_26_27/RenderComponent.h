#pragma once

#include "IComponent.h"

class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& entity);
	~RenderComponent() = default;

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;
};

