#pragma once

#include <iostream>
#include <vector>
#include <string>

struct MissionReward {
	MissionReward() : gold(0), itemName("") {}
	MissionReward(int gold, std::string itemName = "") : gold(gold), itemName(itemName) {}
	int gold;
	std::string itemName;
};

class Mission {
public:
	Mission(std::string id, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed = false) : id(id), targetId(targetId), targetName(targetName), reward(reward), unlocksMissions(unlocksMissions), active(active), completed(completed) {}
	inline virtual std::string ToString() { return ""; }
	inline virtual std::string ToStringReward() 
	{ 
		std::string s = "Reward: ";
		if (reward.gold != 0) s += std::to_string(reward.gold) + " gold";
		if (reward.gold != 0 && reward.itemName != "") s += ", ";
		if (reward.itemName != "") s += reward.itemName;
	}
	std::string id = "";
	std::string targetId = "";
	std::string targetName = "";
	MissionReward reward = MissionReward();
	std::vector<std::string> unlocksMissions = std::vector<std::string>();
	bool active = false;
	bool completed = false;
private:

};