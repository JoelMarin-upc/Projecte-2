#pragma once

#include "StaticEntity.h"

enum class ItemInteractionType {
	DEFAULT,
	PICKUP,
	TOGGLE,
	DIALOGUE
};

class InteractableItem : public StaticEntity {
public:
	InteractableItem() {}
	InteractableItem(ItemInteractionType type);
	virtual ~InteractableItem();

	virtual bool Awake() override;

	virtual bool Start() override;

	virtual bool Update(float dt) override;

	virtual void Draw(float dt) override;

	virtual bool CleanUp() override;

	virtual void OnCollision(Collider* physA, Collider* physB) override;

	virtual void OnCollisionEnd(Collider* physA, Collider* physB) override;

protected:
	virtual void Interact();

	virtual void Pickup();

	virtual void Toggle();

public:
	bool isPlayerInRange = false;
	bool isPicked = false;

protected:
	//Dialogue type by default
	ItemInteractionType itemInteractionType;

	SDL_Texture* pickupIcon;
	const char* pickupIconPath;
	int pickupIconW = 16;
	int pickupIconH = 16;

	//L08 TODO 4: Add a physics to an item
	Collider* pbody;
	Collider* sensorCollider;
};