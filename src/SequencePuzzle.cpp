#include "SequencePuzzle.h"
#include "SequenceButton.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Log.h"
#include "Audio.h"

SequencePuzzle::SequencePuzzle(std::vector<int> correctOrder) : correctOrder(correctOrder)
{
}

void SequencePuzzle::RegisterButton(SequenceButton* button)
{
    buttons.push_back(button);

    button->onPressed = [this](int index) {
        OnButtonPressed(index);
        };
}

void SequencePuzzle::OnButtonPressed(int index)
{
    if (isSolved) return;

    currentSequence.push_back(index);
    int step = currentSequence.size() - 1;

    if (currentSequence[step] != correctOrder[step]) {
        OnFailure();
        return;
    }

    if (currentSequence.size() == correctOrder.size()) {
        OnSuccess();
    }
}

void SequencePuzzle::LoadSounds()
{
    std::string dingFxPath = Engine::GetInstance().audio->GetAudioPath("puzzle", "ding");
    dingFxId = Engine::GetInstance().audio->LoadFx(dingFxPath.c_str());
}

void SequencePuzzle::OnSuccess()
{
    isSolved = true;
    if (rewardItem) rewardItem->active = true;
    if (dingFxId != -1) Engine::GetInstance().audio->PlayFx(dingFxId);
    //for (SequenceButton* btn : buttons) btn->active = false;
}

void SequencePuzzle::OnFailure()
{
    currentSequence.clear();
    //if (dingFxId != -1) Engine::GetInstance().audio->PlayFx(dingFxId);
}