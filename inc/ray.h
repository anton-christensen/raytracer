#ifndef _RAY_
#define _RAY_

#include <float.h>
#include <stdio.h>

#include "vector.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "octree.h"

typedef struct {
	vec origin, direction;
} ray;

ray ray_create(vec origin, vec direction);
vec ray_get_point(ray r, double t);
int ray_aabb_slab_test(double slab_low, double slab_high, double origin, double direction, double *t_near, double *t_far);
int ray_aabb_colides(ray r, Octree* aabb);
int ray_sphere_colides(ray r, Sphere s);
double ray_sphere_get_intersection(ray r, Sphere* s, vec* hit, vec* normal, vec* color);
double ray_plane_get_intersection(ray r, Plane* p, vec* hit, vec* normal, vec* color);
double ray_tri_get_intersection(ray r, Tri* tri, vec* hit, vec* normal, vec* color);
// t = N DOT(O-RO)/N DOT(RD)


void ray_dump(ray r);


#endif