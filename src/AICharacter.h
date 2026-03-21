#pragma once

#include "Character.h"

class AICharacter : public Character {
public:
	AICharacter() {}
	AICharacter(std::string id, std::string name, std::string texturePath, EntityType type) : Character(id, name, texturePath, type) {}
};