#pragma once

#include "Mission.h"

class BringMission : public Mission {
public:
	BringMission(std::string id, std::string targetId, std::string targetName, std::string itemName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed = false) : Mission(id, targetId, targetName, reward, unlocksMissions, active, completed) { this->itemName = itemName; }
	std::string itemName;
	inline std::string ToString() override
	{
		if (completed) return "Brought a " + itemName + " to " + targetName + ". " + ToStringReward();
		else return "Bring a " + itemName + " to " + targetName + ". " + ToStringReward();
	};
private:

};