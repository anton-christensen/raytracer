#ifndef _TRACER_
#define _TRACER_

#include "common.h"
#include "llist.h"
#include "point_light.h"
#include "ray.h"
#include "vector.h"
#include "scene.h"

vec trace_ray(ray r, Scene* scene, int depth, long int* context_hash);
double get_intersection(ray r, Octree* tree_node, vec* hit, vec* normal, vec* color, llist_node** object_node);
vec shade(Scene* scene, ray* r, vec* hit, vec* normal, vec* color, llist_node* object_node, int depth, long int* context_hash);
Material* get_material(llist_node* n);
vec calc_ambient_occlusion(Scene* scene, vec* point, vec* normal);

#endif