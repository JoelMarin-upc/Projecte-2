#pragma once

#include "Mission.h"

class CollectMission : public Mission {
public:
	inline std::string ToString() override
	{
		if (completed) return "Collected " + targetName + ". " + ToStringReward();
		else return "Collect " + targetName + ". " + ToStringReward();
	};
private:

};