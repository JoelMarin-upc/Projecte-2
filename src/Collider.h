#pragma once

#include "Entity.h"
#include "Animation.h"
#include "Timer.h"
#include "Spear.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>

enum class ColliderType {
    PLAYER,
    ITEM,
    HEART,
    COIN,
    RECHARGE,
    KEY,
    SPEAR,
    PLATFORM,
    DEATHZONE,
    ITEMDESTROYER,
    RESPAWNPOINT,
    BOSSSTART,
    DOOR,
    NEXTLEVEL,
    ENEMY,
    UNKNOWN,
    ATTACK,
    SEMIRIGID
    // ..
};

// Small class to return to other modules to track position and rotation of physics bodies
class Collider
{
public:
    Collider() : listener(NULL), body(b2_nullBodyId), ctype(ColliderType::UNKNOWN) {}
    ~Collider() {}

    void  GetPosition(int& x, int& y) const;
    float GetRotation() const;
    bool  Contains(int x, int y) const;
    int   RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
    b2BodyId body;              // id instead of pointer (v3.x)
    Entity* listener;
    ColliderType ctype;
};