#pragma once

#include <list>
#include "Physics.h"
#include "Collider.h"
#include "Engine.h"
#include "Entity.h"

class DynamicEntity : public Entity
{
public:

	DynamicEntity() {}
	DynamicEntity(std::string id, std::string name, std::string texturePath, EntityType type) : Entity(id, name, texturePath, type) { }

	void AddCollider(ColliderType type, SDL_Texture* tex, float xOffset, float yOffset, float wOffset, float hOffset, uint16_t categoryBits, uint16_t maskBits, int* points = nullptr, int size = 0);

public:

};