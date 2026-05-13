#include "PushBox.h"
#include "Engine.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"

bool PushBox::Start()
{
    pickupIconPath = "";
    texture = Engine::GetInstance().textures->Load(texturePath.c_str());
    icon = texture;
    // No pickup icon for this entity.
    pickupIcon = nullptr;

    // Single solid square collider — no sensor needed for movement.
    // We add a sensor only so OnCollision fires when overlapping the plate.
    AddCollider(ColliderType::SQUARE, texture, 0, 0, 0, 0, 1, 1);
    AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 0, 0, 1, 1);

    texW = 32;
    texH = 32;

    pbody = colliders[0];
    pbody->etype = EntityType::PUSH_BOX;
    pbody->listener = this;

    sensorCollider = colliders[1];
    sensorCollider->etype = EntityType::PUSH_BOX;
    sensorCollider->listener = this;

    b2Body_SetFixedRotation(pbody->body, true);

    b2MassData massData;
    massData.mass = 5.0f;
    massData.center = { 0.0f, 0.0f };
    massData.rotationalInertia = 0.0f;
    b2Body_SetMassData(pbody->body, massData);

    spawnPosition = position;

    return true;
}

bool PushBox::Update(float dt)
{
    if (!active) return true;
    Draw(dt);
    return true;
}

void PushBox::Draw(float dt)
{
    if (!active || !pbody) return;

    int x, y;
    pbody->GetPosition(x, y);
    Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
}

bool PushBox::CleanUp()
{
    LOG("Cleanup PushBox");
    Engine::GetInstance().textures->UnLoad(texture);

    if (sensorCollider) {
        Engine::GetInstance().physics->DeletePhysBody(sensorCollider);
        sensorCollider = nullptr;
    }
    if (pbody) {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }
    return true;
}

void PushBox::ResetToSpawn()
{
    if (!pbody) return;

    b2Body_SetLinearVelocity(pbody->body, { 0.0f, 0.0f });
    b2Body_SetAngularVelocity(pbody->body, 0.0f);

    b2Vec2 pos{
        PIXEL_TO_METERS(spawnPosition.getX()),
        PIXEL_TO_METERS(spawnPosition.getY())
    };

    b2Body_SetTransform(pbody->body, pos, b2Rot_identity);
}