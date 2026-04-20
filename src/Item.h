#pragma once

#include "DynamicEntity.h"
#include "Animation.h"
#include <SDL3/SDL.h>

struct SDL_Texture;

class Item : public DynamicEntity
{
public:

	Item(){}
	Item(std::string id, std::string name, std::string texturePath, EntityType type);
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void Draw(float dt);

	bool CleanUp();

	bool Destroy();

public:

	bool isPicked = false;
	SDL_Texture* icon;
	std::string description;

protected:

	int texW, texH;

	//L08 TODO 4: Add a physics to an item
	Collider* pbody;



};
