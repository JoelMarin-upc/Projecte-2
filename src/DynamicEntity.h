#pragma once

#include <list>
#include "Physics.h"
#include "Engine.h"

class DynamicEntity : public Entity
{
public:

	DynamicEntity() {}
	DynamicEntity(EntityType type, SDL_Texture texture) : Entity(type, texture) {}

	void AddCollider(ColliderType type, float xOffset, float yOffset, float wOffset, float hOffset, uint16_t categoryBits, uint16_t maskBits, int* points = nullptr, int size = 0);

public:

};