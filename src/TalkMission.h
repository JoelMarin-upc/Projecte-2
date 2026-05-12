#pragma once

#include "Mission.h"

class TalkMission : public Mission {
public:
	TalkMission(std::string id, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed = false) : Mission(id, targetId, targetName, reward, unlocksMissions, active, completed) {}
	inline std::string ToString() override 
	{ 
		if (completed) return "Talked to " + targetName + ". " + ToStringReward();
		else return "Talk to " + targetName + ". " + ToStringReward();
	};
private:

};