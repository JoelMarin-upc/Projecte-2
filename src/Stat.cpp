#include "Stat.h"

float Stat::setValue(float v)
{
	value = v;
	if (value > maxValue) value = maxValue;
	return value;
}

float Stat::getValue() const
{
	return value * modifier;
}
