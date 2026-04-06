#pragma once

#include "InteractableItem.h"

struct SDL_Texture;

class Lever : public InteractableItem
{
public:

	Lever();
	virtual ~Lever();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void Draw(float dt);

	bool CleanUp();

	void OnCollision(Collider* physA, Collider* physB);

	void OnCollisionEnd(Collider* physA, Collider* physB);

	void Interact();

public:

private:

	SDL_Texture* openLeverTex = nullptr;
	SDL_Texture* closeLeverTex = nullptr;

	bool isOpened = false;
};
