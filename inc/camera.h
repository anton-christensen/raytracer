#ifndef _CAMERA_
#define _CAMERA_

#include "vector.h"
#include "ray.h"

typedef struct {
	vec eye, ul, ur, ll, lr;
	int res_x, res_y;
} camera;

// camera camera_create_alt(vec eye, vec dir, int res_x, int res_y);
camera camera_create(vec eye, vec ul, vec ur, vec ll, vec lr, int res_x, int res_y);
ray camera_get_ray(camera cam, double index_x, double index_y);

void camera_translate(camera* cam, vec t);

#endif