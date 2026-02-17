#pragma once

#include "Entity.h"
#include <SDL3/SDL.h>

struct SDL_Texture;

class Key : public Entity
{
public:

	Key();
	virtual ~Key();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void Draw(float dt);

	bool CleanUp();

	bool Destroy();

	void CreateBody();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Despawn();


public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;

	//L08 TODO 4: Add a physics to an item
	PhysBody* pbody;
};
