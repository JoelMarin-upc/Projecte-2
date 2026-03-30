#pragma once

#include "AICharacter.h"
#include "Textures.h"
//struct SDL_Texture;

enum class NPCInteractionType {
	DEFAULT,
	RECRUIT, //Talk, then recruit into the team
	SHOP, //Talk, then open shop
	DIALOGUE //Talk only
};

class NPC : public AICharacter {
public:
	NPC(){}
	NPC(std::string id, std::string name, std::string texturePath, NPCInteractionType type = NPCInteractionType::DEFAULT) : AICharacter(id, name, texturePath, EntityType::NPC) {
		npcID = id;
		npcInteractionType = type;
	}
	virtual ~NPC();

	bool Awake() override;
	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);
	void Interact();
	void OnDialogEnd();
	void Recruit();
	void OpenShop();
	void OnCollision(Collider* physA, Collider* physB) override;
	void OnCollisionEnd(Collider* physA, Collider* physB) override;
public:
	bool isPlayerInRange = false;
	bool isRecruitConditionFulfilled = false;
private:
	std::string npcID;
	NPCInteractionType npcInteractionType;
	Collider* pbody = nullptr;
	Collider* sensorCollider = nullptr;
};