#pragma once
#include <vector>
#include <string>
#include "Physics.h"
#include "InteractableItem.h"

class SequenceButton;
class Scene;

class SequencePuzzle {
public:
    SequencePuzzle(std::vector<int> correctOrder);

    void RegisterButton(SequenceButton* button);

    void OnButtonPressed(int index);

    bool isSolved = false;


public:
    std::shared_ptr<InteractableItem> rewardItem = nullptr;
private:
    void OnSuccess();
    void OnFailure();

    std::vector<int> correctOrder;
    std::vector<int> currentSequence;
    std::vector<SequenceButton*> buttons;

    
    Vector2D rewardPosition;
};