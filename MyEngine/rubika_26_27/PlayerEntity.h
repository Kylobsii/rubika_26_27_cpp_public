#pragma once

#include "Entity.h"

class Scene;

class PlayerEntity : public Entity
{
public:
	PlayerEntity(std::string name);
	~PlayerEntity();

private:
	void SetupBindings();
	void DestroyBindings();
};

