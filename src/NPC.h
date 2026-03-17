#pragma once

#include "AICharacter.h"
#include "Textures.h"
//struct SDL_Texture;

enum class NPCInteractionType {
	DEFAULT,
	RECRUIT,
	SHOP,
	DIALOGUE
};

class NPC : public AICharacter {
public:
	NPC(){}
	NPC(std::string id, NPCInteractionType type);
	virtual ~NPC();

	bool Awake() override;
	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);
	void Interact();
	void OnCollision(Collider* physA, Collider* physB) override;
	void OnCollisionEnd(Collider* physA, Collider* physB) override;
public:
	bool isPlayerInRange = false;
private:
	std::string npcID;
	NPCInteractionType npcInteractionType;
	Collider* pbody = nullptr;
	Collider* sensorCollider = nullptr;
};