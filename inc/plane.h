#ifndef _PLANE_
#define _PLANE_

#include "vector.h"
#include "material.h"

typedef struct {
	vec p, norm, color;
    Material material;
} Plane;

Plane plane_create(vec p, vec norm, Material m);
void plane_init(Plane* plane, vec p, vec norm, Material m);

#endif