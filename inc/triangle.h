#ifndef _TRIANGLE_
#define _TRIANGLE_

#include "vector.h"
#include "material.h"

typedef struct {
    vec vertex[3], norm, color;
    Material material;
} Tri;

Tri     tri_create(vec vertex[3], vec color, Material m);
void    tri_init(Tri* poly, vec vertex[3], vec color, Material m);
vec     tri_get_normal(Tri* poly);

#endif