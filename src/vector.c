#include "vector.h"

vec vec_create(double x,double y,double z) {
	vec a = {x,y,z};
	return a;
}

vec vec_unit(vec a) {
	return vec_devide(a, vec_norm(a));
}

vec vec_add(vec a, vec b) {
	vec c = {a.x+b.x, a.y+b.y, a.z+b.z};
	return c;
}

vec vec_sub(vec a, vec b) {
	vec c = {a.x-b.x, a.y-b.y, a.z-b.z};
	return c;
}

vec vec_cross(vec a, vec b) {
	vec c = {a.y*b.z-b.y*a.z, a.z*b.x-b.z*a.x, a.x*b.y-b.x*a.y};
	return c;
}

vec vec_mult(vec a, double s) {
	vec b = {a.x*s, a.y*s, a.z*s};
	return b;
}

vec vec_devide(vec a, double s) {
	return vec_mult(a, 1/s);
}

vec vec_project(vec a, vec b) {
	return vec_mult(b, vec_dot(b,a)/pow(vec_norm(b),2));
}

vec vec_reflect(vec a, vec normal) {
	normal = vec_unit(normal);
	// r=a−2(a DOT n)n
	vec refl = vec_sub(a, vec_mult(vec_mult(normal, vec_dot(a, normal)),2));
	return refl;
}

double vec_dot(vec a, vec b) {
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

double vec_norm(vec a) {
	return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}

vec vec_color_mult(vec a, vec b) {
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}


void vec_dump(const char* name, vec v) {
	printf("\t / %10.4lf \\\n", v.x);
	printf("%s =\t | %10.4lf |\n", name, v.y);
	printf("\t \\ %10.4lf /\n", v.z);
}