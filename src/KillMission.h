#pragma once

#include "Mission.h"

class KillMission : public Mission {
public:
	KillMission(std::string id, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed = false) : Mission(id, targetId, targetName, reward, unlocksMissions, active, completed) {}
	inline std::string ToString() override
	{
		if (completed) return "Killed " + targetName + ". " + ToStringReward();
		else return "Kill " + targetName + ". " + ToStringReward();
	};
private:

};