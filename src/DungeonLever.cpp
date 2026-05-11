#include "DungeonLever.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Scene.h"

void DungeonLever::Toggle() {
    if (isToggled) return;

    isToggled = true;
    Engine::GetInstance().sceneManager->currentScene->OnLeverToggled();
}