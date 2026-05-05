#pragma once

#include <iostream>
#include <vector>

struct MissionReward {
	MissionReward() : gold(0), itemName("") {}
	MissionReward(int gold, std::string itemName = "") : gold(gold), itemName(itemName) {}
	int gold;
	std::string itemName;
};

class Mission {
public:
	Mission(std::string id, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed = false) : id(id), targetId(targetId), targetName(targetName), reward(reward), unlocksMissions(unlocksMissions), active(active), completed(completed) {}
	std::string id = "";
	std::string targetId = "";
	std::string targetName = "";
	MissionReward reward = MissionReward();
	std::vector<std::string> unlocksMissions = std::vector<std::string>();
	bool active = false;
	bool completed = false;
private:

};