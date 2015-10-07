#ifndef _SPHERE_
#define _SPHERE_

#include "vector.h"
#include "material.h"

typedef struct {
	vec pos, color;
	double r;
    Material material;
} Sphere;

Sphere 	sphere_create(double x, double y, double z, double r, vec color, Material m);
void 	sphere_init(Sphere* s, vec c, double r, vec color, Material m);
vec    	sphere_get_normal(Sphere* s, vec* p);

#endif