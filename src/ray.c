#include "ray.h"

ray ray_create(vec origin, vec direction) {
	ray r = {origin, vec_unit(direction)};
	return r;
}

vec ray_get_point(ray r, double t) {
	return vec_add(r.origin, vec_mult(r.direction, t));
}

double ray_sphere_get_intersection(ray r, Sphere* s, vec* hit, vec* normal, vec* color) {
	double t1, t2, t;

	// disc = (d DOT (e-c))^2 - (d DOT d)((e-c) DOT (e-c) - R^2)

	vec dist = vec_sub(r.origin, s->pos);
	double d_dot_d = vec_dot(r.direction, r.direction);
	double discriminant = pow( vec_dot(r.direction, dist), 2) - d_dot_d*(vec_dot(dist,dist) - s->r*s->r);
	if(discriminant < 0)
		return -1;

	double a = 1.0/d_dot_d;
	double b = vec_dot(vec_mult(r.direction,-1), dist);

	t1 = a*(b+sqrt(discriminant));
	t2 = a*(b-sqrt(discriminant));

	t = (t1 < t2 || t2 < 0 ? t1 : t2);

	if(t > 0) {
		// calculate hit
		vec temp = ray_get_point(r, t);
		hit->x = temp.x;
		hit->y = temp.y;
		hit->z = temp.z;

		temp = sphere_get_normal(s, hit);
		normal->x = temp.x;
		normal->y = temp.y;
		normal->z = temp.z;
		if(vec_norm(dist) < s->r)
			*normal = vec_mult(*normal, -1);

		color->x = s->color.x;
		color->y = s->color.y;
		color->z = s->color.z;

	}

	return t;
}

double ray_plane_get_intersection(ray r, Plane* p, vec* hit, vec* normal, vec* color) {
	double t = (vec_dot(vec_sub(p->p,r.origin), p->norm)) / vec_dot(r.direction, p->norm);

	if(t > 0) {
		// calculate hit
		vec temp = ray_get_point(r, t);
		hit->x = temp.x;
		hit->y = temp.y;
		hit->z = temp.z;

		normal->x = p->norm.x;
		normal->y = p->norm.y;
		normal->z = p->norm.z;

		color->x = p->color.x;
		color->y = p->color.y;
		color->z = p->color.z;
	}
	return t;
}

double ray_tri_get_intersection(ray r, Tri* tri, vec* hit, vec* normal, vec* color) {
	double t = (vec_dot(vec_sub(tri->vertex[0],r.origin), tri->norm)) / vec_dot(r.direction, tri->norm);

	if(t > 0) {
		vec p = ray_get_point(r, t);
		vec u = vec_sub(tri->vertex[1], tri->vertex[0]);
		vec v = vec_sub(tri->vertex[2], tri->vertex[0]);
		vec w = vec_sub(p, tri->vertex[0]);

		double uv = vec_dot(u,v);
		double wv = vec_dot(w,v);
		double wu = vec_dot(w,u);
		double uu = vec_dot(u,u);
		double vv = vec_dot(v,v);
		double denom = (uv*uv - uu*vv);

		double s1 = (uv*wv - vv*wu) / denom;
		double s2 = (uv*wu - uu*wv) / denom;

		if(s1 >= 0 && s2 >= 0 && s1+s2 <= 1) {
	
			// calculate hit
			hit->x = p.x;
			hit->y = p.y;
			hit->z = p.z;

			if(vec_dot(tri->norm, vec_sub(r.origin, p)) < 0)
				tri->norm = vec_mult(tri->norm, -1);
			normal->x = tri->norm.x;
			normal->y = tri->norm.y;
			normal->z = tri->norm.z;

			color->x = tri->color.x;
			color->y = tri->color.y;
			color->z = tri->color.z;
			
			return t;
		}
	}
	return -1;
}


void ray_dump(ray r) {
	printf("Ray: e + t*d\n");
	vec_dump("e", r.origin);
	printf("\n");
	vec_dump("d", r.direction);
	printf("\n");
	printf("\n");
	printf("\n");
}