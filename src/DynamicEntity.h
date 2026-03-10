#pragma once

#include <list>
#include "Physics.h"
#include "Collider.h"
#include "Engine.h"

class DynamicEntity : public Entity
{
public:

	DynamicEntity() {}
	DynamicEntity(EntityType type) : Entity(type) { }

	void AddCollider(ColliderType type, SDL_Texture* tex, float xOffset, float yOffset, float wOffset, float hOffset, uint16_t categoryBits, uint16_t maskBits, int* points = nullptr, int size = 0);

public:

};