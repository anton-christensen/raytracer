#include "triangle.h"

Tri tri_create(vec vertex[3], vec color, Material m) {
    Tri poly;
    int i;
    for(i = 0; i < 3; i++)
        poly.vertex[i] = vertex[i];

    poly.color = color;
    poly.material = m;

    poly.norm = vec_unit(vec_cross(vec_sub(vertex[1], vertex[0]), vec_sub(vertex[2],vertex[0])));

    return poly;
}

void tri_init(Tri* poly, vec vertex[3], vec color, Material m) {
    int i;
    for(i = 0; i < 3; i++)
        poly->vertex[i] = vertex[i];

    poly->color = color;
    poly->material = m;

    poly->norm = vec_unit(vec_cross(vec_sub(vertex[1], vertex[0]), vec_sub(vertex[2],vertex[0])));
}

vec tri_get_normal(Tri* poly) {
    return poly->norm;
}
