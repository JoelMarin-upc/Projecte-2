#pragma once
#include <string>
#include <vector>
#include "DialogAnswer.h"

class DialogNode
{
public:
	std::string id;
	std::string text;
	std::vector<DialogAnswer*> answers;
	bool first;
	
private:
	
};