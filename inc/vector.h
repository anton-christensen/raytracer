#ifndef _VECTOR_
#define _VECTOR_

#include <math.h>
#include <stdio.h>

typedef struct {
	double x,y,z;
} vec;

vec vec_create(double x,double y,double z);
vec vec_unit(vec a);
vec vec_add(vec a, vec b);
vec vec_sub(vec a, vec b);
vec vec_cross(vec a, vec b);
vec vec_mult(vec a, double s);
vec vec_devide(vec a, double s);
vec vec_project(vec a, vec b);
vec vec_reflect(vec a, vec normal);
double vec_dot(vec a, vec b);
double vec_norm(vec a);

vec vec_color_mult(vec a, vec b);

void vec_dump(const char* name, vec v);

#endif