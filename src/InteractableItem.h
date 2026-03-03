#pragma once

#include "Item.h"

class InteractableItem : Item {
public:

	InteractableItem();
	virtual ~InteractableItem();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void Draw(float dt);

	bool CleanUp();

	bool Destroy();

	void Interact();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	SDL_Texture* pickupTexture;
	const char* texturePath;
	const char* pickupTexturePath;
	int texW, texH;
	int pickupTexW, pickupTexH;

	//L08 TODO 4: Add a physics to an item
	Collider* pbody;
};