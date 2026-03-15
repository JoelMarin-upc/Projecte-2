#pragma once

#include "DialogNode.h"

class DialogTree
{
public:
	std::string id;
	std::string characterId;
	std::vector<DialogNode*> nodes;
	bool done;
	int order;
	DialogNode* currentNode;

private:
	
};