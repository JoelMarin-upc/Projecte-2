#pragma once

#include "Mission.h"

class TalkMission : public Mission {
public:
	inline std::string ToString() override 
	{ 
		if (completed) return "Talked to " + targetName + ". " + ToStringReward();
		else return "Talk to " + targetName + ". " + ToStringReward();
	};
private:

};