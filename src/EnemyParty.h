#pragma once

#include <iostream>
#include <vector>

class Player;
class Enemy;

class EnemyParty {
public:
	EnemyParty(std::shared_ptr<Enemy> _leader);
	void AddMember(std::shared_ptr<Enemy> member);
	bool CanAddMember() const;

	std::shared_ptr<Enemy> leader;
	std::vector<std::shared_ptr<Enemy>> members;
	
	const int maxMembers = 4;
};