#ifndef _CAMERA_
#define _CAMERA_

#include <math.h>

#include "common.h"
#include "llist.h"
#include "ray.h"
#include "tracer.h"
#include "vector.h"
#include "scene.h"

typedef struct {
	vec eye, ul, ur, ll, lr;
	int res_x, res_y;
} camera;

camera camera_create(vec eye, vec ul, vec ur, vec ll, vec lr, int res_x, int res_y);
ray camera_get_ray(camera* cam, double index_x, double index_y);
void camera_translate(camera* cam, vec t);
void camera_render(camera* cam, char* outfile_path, Scene* scene);
void camera_auto_position(camera* cam, llist* geometry);

#endif