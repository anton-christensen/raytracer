#include "plane.h"

Plane plane_create(vec p, vec norm, Material m) {
	return (Plane){p,vec_unit(norm), (vec){1,1,1}, m};
}

void plane_init(Plane* plane, vec p, vec norm, Material m) {
	plane->p = p;
	plane->norm = norm;
	plane->color = (vec){1,1,1};
    plane->material = m;
}
