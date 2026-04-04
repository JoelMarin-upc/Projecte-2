#pragma once

#include <iostream>
#include <vector>

class Player;
class NPC;

class Party {
public:
	Party(std::shared_ptr<Player> leader);
	void AddMember(std::shared_ptr<NPC> member);
	void RemoveMember(std::string id);
	bool CanAddMember() const;

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<NPC>> members;
	
	const int maxMembers = 3;
};