#pragma once

#include "Input.h"
#include "Render.h"

enum class EntityType
{
	PLAYER,
	ITEM,
	HEART,
	COIN,
	RECHARGE,
	KEY,
	SPEAR,
	ENEMY_AIR,
	ENEMY_GROUND,
	BOSS,
	UNKNOWN
};

class Collider;

class Entity : public std::enable_shared_from_this<Entity>
{
public:

	Entity() {}
	Entity(EntityType type) : type(type), active(true) {}

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

	std::string name;
	EntityType type;
	bool active = true;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	Vector2D position;       
	bool renderable = true;
	bool started = false;

	
};