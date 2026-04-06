#include <iostream>

class Stat {
public: 
	Stat(std::string name, float value, float maxValue) : name(name), value(value), maxValue(maxValue), modifier(1.f), modifierTurnsLeft(-1) {};

	std::string name;
	float value;
	float maxValue;
	float modifier;
	int modifierTurnsLeft;

	float setValue(float v);
	float getValue() const;
};