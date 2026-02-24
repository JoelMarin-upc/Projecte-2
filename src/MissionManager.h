#pragma once

#include "Mission.h"
#include <list>
#include <memory>

class MissionManager
{
public:

	MissionManager();

	// Destructor
	virtual ~MissionManager();

	// Called before render is available
	bool Awake();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	std::shared_ptr<Entity> CreateMission(Mission* mission);

public:

	std::list<std::shared_ptr<Mission>> missions;
	bool paused = false;

};
