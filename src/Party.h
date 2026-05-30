#pragma once

#include <iostream>
#include <vector>

class Player;
class NPC;
class Character;

class Party {
public:
	Party(std::shared_ptr<Player> leader);
	void AddMember(std::shared_ptr<NPC> member, bool write = false);
	void RemoveMember(std::string id, bool write = false);
	bool CanAddMember() const;

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<NPC>> members;
	std::vector<std::shared_ptr<Character>> allMembers;
	
	const int maxMembers = 3;
};