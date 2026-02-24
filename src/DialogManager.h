#pragma once

#include "DialogTree.h"
#include <list>
#include <memory>

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

	void CreateDialog(DialogTree* dialog);

	std::list<std::shared_ptr<DialogTree>> dialogues;
	bool paused = false;

};
