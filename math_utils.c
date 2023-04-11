#include "collider.h"

float maxf(float a, float b)
{
	if (a < b)
		return b;
	return a;
}

float minf(float a, float b)
{
	if (a > b)
		return b;
	return a;
}