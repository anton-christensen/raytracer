#ifndef _SCENE_
#define _SCENE_

#include "common.h"
#include "octree.h"
#include "llist.h"

typedef struct {
    Octree tree;
    llist* lights;
} Scene;

void scene_build(Scene* scene, llist* geometry, llist* lights);
void scene_build_octree(Octree* node, vec dimensions, vec center);
void get_subnode_pos_and_size(vec parrent_dimensions, vec parrent_center, int index, vec* dimensions, vec* center);

#endif