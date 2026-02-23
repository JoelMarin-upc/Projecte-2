#pragma once
#include "Module.h"
#include "Entity.h"
#include <list>
#include <cmath>           // for floor in METERS_TO_PIXELS
#include <box2d/box2d.h>   // Box2D 3.x single header
#include <unordered_set>

#define GRAVITY_X 0.0f
#define GRAVITY_Y -10.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL  0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * (m)))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * (p))

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

// types of bodies
enum bodyType {
    DYNAMIC,
    STATIC,
    KINEMATIC
};

// Module --------------------------------------
class Physics : public Module
{
public:

    // Constructors & Destructors
    Physics();
    ~Physics();

    // Main module steps
    bool Start();
    bool PreUpdate();
    bool PostUpdate(float dt);
    bool CleanUp();

    void CreateWorld();

    // Create basic physics objects
    Collider* CreateRectangle(int x, int y, int width, int height, bodyType type, uint16_t categoryBits, uint16_t maskBits);
    Collider* CreateCircle(int x, int y, int radious, bodyType type, uint16_t categoryBits, uint16_t maskBits);
    Collider* CreateRectangleSensor(int x, int y, int width, int height, bodyType type, uint16_t categoryBits, uint16_t maskBits);
    Collider* CreateCircleSensor(int x, int y, int radious, bodyType type, uint16_t categoryBits, uint16_t maskBits);
    Collider* CreateChain(int x, int y, int* points, int size, bodyType type, uint16_t categoryBits, uint16_t maskBits);

    // Invoked from our event processing
    void BeginContact(b2ShapeId shapeA, b2ShapeId shapeB);
    void EndContact(b2ShapeId shapeA, b2ShapeId shapeB);

    void DeletePhysBody(Collider* physBody);
    bool IsPendingToDelete(Collider* physBody);

    // --- Velocity helpers (thin wrappers over Box2D 3.x C API)
    b2Vec2 GetLinearVelocity(const Collider* p) const;
    float  GetXVelocity(const Collider* p) const;
    float  GetYVelocity(const Collider* p) const;

    void   SetLinearVelocity(Collider* p, const b2Vec2& v) const;
    void   SetLinearVelocity(Collider* p, float vx, float vy) const;
    void   SetXVelocity(Collider* p, float vx) const;
    void   SetYVelocity(Collider* p, float vy) const;

    // --- Impulse helper (handy for jumps/dashes)
    void   ApplyLinearImpulseToCenter(Collider* p, float ix, float iy, bool wake = true) const;

    void   DestroyBody(Collider* p) const;

    b2Transform GetTransform(Collider* p);
    void MoveBody(Collider* p, b2Vec2 pos, b2Rot rot);
    
    void SetGravityScale(Collider* p, float scale);

private:
    // helpers
    static b2BodyType ToB2Type(bodyType t);
    static void* ToUserData(Collider* p) { return (void*)p; }
    static Collider* FromUserData(void* ud) { return (Collider*)ud; }
    static Collider* BodyToPhys(b2BodyId b) { return FromUserData(b2Body_GetUserData(b)); }

    // --- Debug draw callbacks (Box2D 3.1 signatures)
    static void DrawSegmentCb(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* ctx);
    static void DrawPolygonCb(const b2Vec2* verts, int count, b2HexColor color, void* ctx);
    static void DrawSolidPolygonCb(b2Transform xf, const b2Vec2* verts, int count, float radius, b2HexColor color, void* ctx);
    static void DrawCircleCb(b2Vec2 center, float radius, b2HexColor color, void* ctx);
    static void DrawSolidCircleCb(b2Transform xf, float radius, b2HexColor color, void* ctx);

    // --- Defensive no-op stubs (so Box2D never calls a null fn ptr)
    static void DrawSolidCapsuleStub(b2Vec2 a, b2Vec2 b, float r, b2HexColor c, void* ctx);
    static void DrawPointStub(b2Vec2 p, float size, b2HexColor c, void* ctx);
    static void DrawStringStub(b2Vec2 p, const char* s, b2HexColor c, void* ctx);
    static void DrawTransformStub(b2Transform xf, void* ctx);

public:

    bool paused = false;

private:

    // Debug mode
    bool debug;

    // Box2D World (id instead of pointer)
    b2WorldId world;

    // List of physics bodies
    std::unordered_set<Collider*> bodiesToDelete;
};
