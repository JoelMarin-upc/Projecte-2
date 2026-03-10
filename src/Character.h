#pragma once

#include "DynamicEntity.h"

class Character : public DynamicEntity {
public:
	Character() {}
	Character(EntityType type) : DynamicEntity(type) {}
};