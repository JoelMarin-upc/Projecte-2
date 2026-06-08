#include "BossNPC.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Log.h"

BossNPC::~BossNPC()
{
    if (bossParty) {
        delete bossParty;
        bossParty = nullptr;
    }
}

bool BossNPC::Start()
{
    NPC::Start();

    bossParty = new EnemyParty(nullptr);
    bossParty->AddBossMember(std::dynamic_pointer_cast<Character>(shared_from_this()));

    return true;
}

bool BossNPC::Update(float dt)
{
    if (!active) return true;
    if (phase == BossPhase::DONE) return true;

    if (phase == BossPhase::IN_COMBAT || phase == BossPhase::OUTRO_DIALOGUE) {
        if (!colliders.empty() && colliders[0] && sensorCollider) {
            int x, y;
            colliders[0]->GetPosition(x, y);
            b2Body_SetTransform(sensorCollider->body,{ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, b2Body_GetRotation(sensorCollider->body));
        }
        Draw(dt);
        return true;
    }

    return NPC::Update(dt);
}

void BossNPC::OnDialogEnd()
{
    switch (phase)
    {
    case BossPhase::INTRO_DIALOGUE:
    {
        phase = BossPhase::IN_COMBAT;
        Scene* scene = Engine::GetInstance().sceneManager->currentScene;
        if (scene && bossParty) {
            scene->StartBossCombat(bossParty);
        }
        break;
    }

    case BossPhase::OUTRO_DIALOGUE:
    {
        phase = BossPhase::DONE;

        Scene* scene = Engine::GetInstance().sceneManager->currentScene;
        if (!scene) break;

        active = false;

        scene->TriggerNiaEnding(endingChoice);
        break;
    }

    default:
        break;
    }
}

void BossNPC::OnCombatWon()
{
    phase = BossPhase::OUTRO_DIALOGUE;
    Scene* scene = Engine::GetInstance().sceneManager->currentScene;
    if (scene) {
        scene->StartDialog(outroDlgId.empty() ? id : outroDlgId);
    }
}