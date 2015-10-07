
#include "point_light.h"

Pointl pointl_create(double x, double y, double z, double intensity) {
	Pointl l;
	l.pos = (vec){x,y,z};
	l.color = vec_unit((vec){intensity, intensity, intensity});
	return l;
}

void pointl_init(Pointl* l, vec pos, double intense) {
	l->pos = pos;
	l->color = (vec){intense,intense,intense};
}