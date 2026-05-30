#pragma once

#include "Mission.h"

class ReachMission : public Mission {
public:
	ReachMission(std::string id, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed = false) : Mission(id, targetId, targetName, reward, unlocksMissions, active, completed) {}
	inline std::string ToString() override
	{
		if (completed) return "Reached " + targetName + ". " + ToStringReward();
		else return "Reach " + targetName + ". " + ToStringReward();
	};
private:

};