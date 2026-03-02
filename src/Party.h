#include <iostream>
#include "NPC.h"
#include "Player.h"

class Party {
public:
	Player player;
	std::list<NPC> members;
};