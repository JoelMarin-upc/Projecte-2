#pragma once

#include "Mission.h"
#include "TalkMission.h"
#include "ReachMission.h"
#include "KillMission.h"
#include "CollectMission.h"
#include "BringMission.h"
#include <list>
#include <vector>

enum MissionType {
	TALK,
	REACH,
	KILL,
	COLLECT,
	BRING
};

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
	void LoadMissions();
	Mission* AddMission(MissionType type, std::string missionId, std::string targetId, std::string targetName, std::string itemName, MissionReward reward = MissionReward(), std::vector<std::string> unlocksMissions = std::vector<std::string>(), bool active = false, bool completed = false);
	Mission* ActivateMission(std::string missionId);
	Mission* CompleteMission(std::string missionId);
	std::vector<Mission*> GetActiveMissions();
	bool IsMissionCompleted(std::string missionId);

	template<typename T>
	inline std::vector<T*> GetMissions(bool onlyActive)
	{
		std::vector<T*> result;

		for (const auto& m : missions) {
			if (onlyActive && !m->active) continue;
			if (T* casted = dynamic_cast<T*>(m)) result.push_back(casted);
		}

		return result;
	}

public:

	std::list<Mission*> missions;
	bool paused = false;

};
