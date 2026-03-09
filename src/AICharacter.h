#pragma once

#include "Character.h"

class AICharacter : public Character {
public:
	AICharacter() {}
	AICharacter(EntityType type) : Character(type) {}
};