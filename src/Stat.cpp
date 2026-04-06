#include "Stat.h"

float Stat::setValue(float v)
{
	value = v;
	if (value > maxValue) value = maxValue;
	return value;
}

float Stat::getValue() const
{
	if (modifierTurnsLeft > 0) return value * modifier;
	return value;
}
