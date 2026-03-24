#pragma once

#include "DynamicEntity.h"

class Character : public DynamicEntity {
public:
	Character() {}
	Character(std::string id, std::string name, std::string texturePath, EntityType type) : DynamicEntity(id, name, texturePath, type) {}
};