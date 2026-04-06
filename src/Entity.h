#pragma once

#include <vector>
#include "Input.h"
#include "Render.h"
#include <SDL3/SDL.h>

enum class EntityType
{
	PLAYER = 1,
	WALL = 2,
	NPC = 3,
	INTERACTABLE_ITEM = 4,
	ENEMY = 5,
	UNKNOWN
};

class Collider;

class Entity : public std::enable_shared_from_this<Entity>
{
public:

	Entity() {}
	Entity(std::string _id, std::string _name, std::string _texturePath, EntityType _type) : id(_id), name(_name), texturePath(_texturePath), type(_type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual void Draw(float dt) { }

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Destroy()
	{
		return true;
	}

	void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}


	virtual void OnCollision(Collider* physA, Collider* physB) {

	};

	virtual void OnCollisionEnd(Collider* physA, Collider* physB) {

	};


public:

	std::string id;
	std::string name;
	EntityType type;
	bool active = true;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	Vector2D position;       
	bool renderable = true;
	bool started = false;

	int texW, texH;

	std::vector<Collider*> colliders;
	SDL_Texture* texture;
	std::string texturePath;
	
};