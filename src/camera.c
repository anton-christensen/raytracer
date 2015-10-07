#include "camera.h"
/*
camera camera_create_alt(vec eye, vec dir, int res_x, int res_y) {
	camera c;
	c.eye = eye;
	c.res_x = res_x;
	c.res_y = res_y;

	dir = vec_unit(dir);

	c.ul = (vec){-1, 1, -1};
	vec t;
	vec t.x = vec_dot(ul, (vec){1,		0,		 0});
	vec t.y = vec_dot(ul, (vec){0,		cos(),	-sin()});
	vec t.z = vec_dot(ul, (vec){1,		sin(),	 cos()});

	return c;	
}
*/

camera camera_create(vec eye, vec ul, vec ur, vec ll, vec lr, int res_x, int res_y) {
	camera c = {eye, ul, ur, ll, lr, res_x, res_y};
	return c;
}

ray camera_get_ray(camera cam, double index_x, double index_y) {
	vec p;
	double x = (double)index_x/cam.res_x;
	double y =1-(double)index_y/cam.res_y;
	
	p = vec_add(vec_mult(vec_add(vec_mult(cam.ll,1-y), vec_mult(cam.ul,y)),1-x), vec_mult(vec_add(vec_mult(cam.lr,1-y), vec_mult(cam.ur,y)),x));
	ray r = ray_create(cam.eye, vec_sub(p, cam.eye));
	return r;
}