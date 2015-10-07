#ifndef _POINT_LIGHT_
#define _POINT_LIGHT_

#include "vector.h"

typedef struct {
	vec pos, color;
} Pointl;

Pointl pointl_create(double x, double y, double z, double intensity);
void pointl_init(Pointl* l, vec pos, double intense);

#endif