#ifndef _COMMON_
#define _COMMON_

#include "llist.h"
#include "vector.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define ROWS 960
#define COLS 960

#define ANTIALIAS_FACTOR 1 // max 4
#define REFLECTION_DEPTH 1
#define AMBIENT_OCCLUSION_SAMPLES 1 // 128 samples
#define SMART_ANTIALIAS 0

enum primitive {
    SPHERE,
    PLANE,
    TRIANGLE
};
enum light {
    POINT
};

void calc_space_of_geometry(llist* geometry, vec* size, vec* position);

#endif