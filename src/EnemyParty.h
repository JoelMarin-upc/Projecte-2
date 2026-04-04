#include <iostream>
#include "Enemy.h"

class EnemyParty {
public:
	EnemyParty(std::shared_ptr<Enemy> _leader);
	std::shared_ptr<Enemy> leader;
	std::vector<std::shared_ptr<Enemy>> members;
};