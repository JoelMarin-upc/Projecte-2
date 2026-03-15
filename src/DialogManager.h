#pragma once

#include "DialogTree.h"
#include <list>
#include <memory>
#include <SDL3/SDL.h>
#include "UILabel.h"
#include "UIButton.h"

class DialogManager
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

	void ShowDialog(DialogTree* dialog);

	std::list<std::shared_ptr<DialogTree>> dialogs;
	bool paused = false;

private:

	DialogTree* currentDialog;
	SDL_Texture dialogBox;
	UILabel dialogText;
	UIButton answer1;
	UIButton answer2;
	UIButton answer3;
	UIButton answer4;

};
