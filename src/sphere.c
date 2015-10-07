#include "sphere.h"

Sphere sphere_create(double x, double y, double z, double r, vec color, Material m) {
	vec o = {x,y,z};
	Sphere s = {o, color, r, m};
    return s;
}

void sphere_init(Sphere* s, vec c, double r, vec color, Material m) {
	s->pos = c;
	s->color = color;
	s->r = r;
    s->material = m;
}

vec sphere_get_normal(Sphere* s, vec* p) {
	return vec_unit(vec_sub(*p, s->pos));
}