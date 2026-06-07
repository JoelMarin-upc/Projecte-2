#pragma once

#include "AICharacter.h"
#include "Textures.h"

class Party;

//struct SDL_Texture;

enum class NPCInteractionType {
	DEFAULT,
	RECRUIT, //Talk, then recruit into the team
	SHOP, //Talk, then open shop
	DIALOGUE, //Talk only
	BOSS
};

class NPC : public AICharacter {
public:
	NPC(){}
	NPC(std::string id, std::string name, std::string texturePath, std::string combatTexturePath, NPCInteractionType type = NPCInteractionType::DEFAULT, std::string recuitMissionId = "", std::string infectedTexturePath = "") : AICharacter(id, name, texturePath, combatTexturePath, EntityType::NPC), infectedTexturePath(infectedTexturePath) {
		npcInteractionType = type;
		this->recuitMissionId = recuitMissionId;
	}
	virtual ~NPC();

	virtual bool Awake() override;
	virtual bool Start() override;

	bool Update(float dt);
	void Draw(float dt);
	virtual bool CleanUp();
	void CreateColliders();
	void Move();
	void HandleAnimations(b2Vec2 velocity);
	void LoadAnimations();
	void Interact();
	virtual void OnDialogEnd();
	void Recruit();
	void OpenShop();
	void OnCollision(Collider* physA, Collider* physB) override;
	void OnCollisionEnd(Collider* physA, Collider* physB) override;
	SDL_Texture* GetCombatTexture() const override;

public:
	bool isPlayerInRange = false;
	bool isRecruitConditionFulfilled = false;
	Party* party;
	int partyIndex = 1;
	std::string animationsPath;
	std::string facing = "down";
	std::string currentAnimation = "";
	bool isFacingRight = false;
	std::string infectedTexturePath;
	SDL_Texture* infectedTexture = nullptr;
	SDL_Texture* combatTexture2 = nullptr;
	std::string combatTexture2Path;
protected:
	AnimationSet anims;
	NPCInteractionType npcInteractionType;
	Collider* pbody = nullptr;
	Collider* sensorCollider = nullptr;
	std::string recuitMissionId;
};