#pragma once

#include "Mission.h"

class CollectMission : public Mission {
public:
	CollectMission(std::string id, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed = false) : Mission(id, targetId, targetName, reward, unlocksMissions, active, completed) {}
	inline std::string ToString() override
	{
		if (completed) return "Collected " + targetName + ". " + ToStringReward();
		else return "Collect " + targetName + ". " + ToStringReward();
	};
private:

};