#pragma once

#include "AICharacter.h"
#include "Textures.h"
//struct SDL_Texture;

class NPC : public AICharacter {
public:
	NPC(std::string id, std::string name, std::string texturePath) : AICharacter(id, name, texturePath, EntityType::NPC) {}

	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);
};