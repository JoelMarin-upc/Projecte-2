#pragma once

#include "Mission.h"

class BringMission : public Mission {
public:
	std::string itemName;
	inline std::string ToString() override
	{
		if (completed) return "Brought a " + itemName + " to " + targetName + ". " + ToStringReward();
		else return "Bring a " + itemName + " to " + targetName + ". " + ToStringReward();
	};
private:

};