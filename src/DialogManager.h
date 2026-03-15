#pragma once

#include "DialogTree.h"
#include <list>
#include <memory>
#include <SDL3/SDL.h>
#include "UILabel.h"
#include "UIButton.h"

class DialogManager : public Module
{
public:
	DialogManager();

	// Destructor
	virtual ~DialogManager();

	// Called before render is available
	bool Awake();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void LoadDialogs();

	void SetCurrentDialog(std::string characterId);

	void ShowDialog();

	bool OnUIMouseClickEvent(UIElement* uiElement);

	std::list<DialogTree*> dialogs;
	bool paused = false;

private:

	DialogTree* currentDialog;
	SDL_Texture dialogBox;
	std::shared_ptr<UILabel> dialogText;
	std::shared_ptr<UIButton> answer1;
	std::shared_ptr<UIButton> answer2;
	std::shared_ptr<UIButton> answer3;
	std::shared_ptr<UIButton> answer4;

	enum DIALOG_UIID {
		LABEL,
		ANSWER1,
		ANSWER2,
		ANSWER3,
		ANSWER4
	};
};
