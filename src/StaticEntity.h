#pragma once

#include <list>
#include "Physics.h"
#include "Engine.h"

class StaticEntity : public Entity
{
public:

	StaticEntity() {}
	StaticEntity(std::string id, std::string name, std::string texturePath, EntityType type) : Entity(id, name, texturePath, "", type) {}

	void AddCollider(ColliderType type, SDL_Texture* tex, float xOffset, float yOffset, float wOffset, float hOffset, uint16_t categoryBits, uint16_t maskBits, int* points = nullptr, int size = 0);

public:

};