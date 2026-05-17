#pragma once

#include "Character.h"

class AICharacter : public Character {
public:
	AICharacter() {}
	AICharacter(std::string id, std::string name, std::string texturePath, std::string combatTexturePath, EntityType type) : Character(id, name, texturePath, combatTexturePath, type) {}
};