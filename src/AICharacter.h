#pragma once

#include "Character.h"

class AICharacter : public Character {
public:
	AICharacter() {}
	AICharacter(std::string id, EntityType type) : Character(id, type) {}
};