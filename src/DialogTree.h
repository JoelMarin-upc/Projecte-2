#pragma once

#include "DialogNode.h"

class DialogTree
{
public:
	std::string id;
	std::string characterId;
	std::string characterName;
	std::string missionId;
	std::vector<DialogNode*> nodes;
	bool done;
	int order;
	bool isRepeatable;
	DialogNode* currentNode;

private:
	
};