#pragma once

#include <iostream>
#include <list>

class Player;
class NPC;

class Party {
public:
	Party(Player* leader);
	void AddMember(std::shared_ptr<NPC> member);
	void RemoveMember(std::string id);
	bool CanAddMember() const;
	Player* player;
	std::list<std::shared_ptr<NPC>> members;
	const int maxMembers = 3;
};