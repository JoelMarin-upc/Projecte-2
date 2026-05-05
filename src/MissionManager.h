#pragma once

#include "Mission.h"
#include <list>
#include <vector>

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
	void SaveMissions();
	Mission* AddMission(std::string missionId, std::string targetId, std::string targetName, MissionReward reward = MissionReward(), std::vector<std::string> unlocksMissions = std::vector<std::string>());
	MissionReward CompleteMission(std::string missionId);
	std::vector<Mission*> GetActiveMissions();

	template<typename T>
	inline std::vector<T*> GetMissions(bool onlyActive)
	{
		std::vector<T*> result;

		for (const auto& m : missions) {
			if (onlyActive && !m->active) continue;
			if (T* casted = static_cast<T*>(m)) result.push_back(casted);
		}

		return result;
	}

public:

	std::list<Mission*> missions;
	bool paused = false;

};
