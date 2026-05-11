#include "DungeonExit.h"
#include "Engine.h"
#include "Physics.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Log.h"

void DungeonExit::Unlock()
{
    isToggled = true;

    if (pbody) {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }
    LOG("Dungeon exit unlocked!");
}

void DungeonExit::Interact()
{
    if (isToggled) {
        Engine::GetInstance().sceneManager->currentScene->StartDialog("exit_open");
    }
    else {
        Engine::GetInstance().sceneManager->currentScene->StartDialog("exit_locked");
    }
}