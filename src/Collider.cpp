#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Collider.h"

void Collider::GetPosition(int& x, int& y) const
{
    b2Vec2 pos = b2Body_GetPosition(body);
    x = METERS_TO_PIXELS(pos.x);
    y = METERS_TO_PIXELS(pos.y);
}

float Collider::GetRotation() const
{
    b2Transform xf = b2Body_GetTransform(body);
    return RADTODEG * b2Rot_GetAngle(xf.q);
}

bool Collider::Contains(int x, int y) const
{
    // World-space point in meters
    const b2Vec2 p = { PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) };

    // Get all shapes attached to this body
    const int shapeCount = b2Body_GetShapeCount(body);
    if (shapeCount == 0) return false;

    std::vector<b2ShapeId> shapes(shapeCount);
    b2Body_GetShapes(body, shapes.data(), shapeCount);

    // Test point against each shape
    for (int i = 0; i < shapeCount; ++i)
    {
        if (b2Shape_TestPoint(shapes[i], p))
            return true;
    }
    return false;
}

int Collider::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
    const b2Vec2 p1 = { PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1) };
    const b2Vec2 p2 = { PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2) };
    const b2Vec2 d = { p2.x - p1.x, p2.y - p1.y };

    b2WorldId world = b2Body_GetWorld(body);
    b2QueryFilter qf = { 0x0004,0x0001 }; //b2DefaultQueryFilter();

    const b2RayResult res = b2World_CastRayClosest(world, p1, d, qf);
    if (!res.hit) return -1;

    normal_x = res.normal.x;
    normal_y = res.normal.y;

    const float fx = float(x2 - x1);
    const float fy = float(y2 - y1);
    const float distPixels = sqrtf(fx * fx + fy * fy);
    return int(floorf(res.fraction * distPixels));
}