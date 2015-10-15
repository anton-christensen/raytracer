#ifndef _OCTREE_
#define _OCTREE_

#include "llist.h"

typedef struct _Octree {
    struct _Octree* octets; // always size 8 but can't be array due to incomplete type error
    llist* primitives;
} Octree;

#endif