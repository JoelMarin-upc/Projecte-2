#pragma once

#include "DynamicEntity.h"

class Character : public DynamicEntity {
public:
	Character() {}
	Character(std::string id, EntityType type) : DynamicEntity(id, type) {}
};