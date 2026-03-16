#pragma once

#include "AICharacter.h"
#include "Textures.h"
//struct SDL_Texture;

class NPC : public AICharacter {
public:
	NPC(std::string id) : AICharacter(id, EntityType::NPC) {}

	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);
};