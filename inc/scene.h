#ifndef _SCENE_
#define _SCENE_

#include "octree.h"

typedef struct {
    Octree tree,
    vec center,
    vec dimensions,
    llist* lights,
    camera cam
} Scene;

#endif