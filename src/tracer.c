#include "tracer.h"

vec trace_ray(ray r, Scene* scene, int depth, long int* context_hash) {
    *context_hash = 0;

    if(depth > REFLECTION_DEPTH)
        return (vec){0,0,0};
    double t;
    vec hit, normal, color;
    llist_node *n;
    
    t = get_intersection(r, &(scene->tree), &hit, &normal, &color, &n);

    if(t < 0) {
        return (vec){0.9,0.9,0.9};
    }

    return shade(scene, &r, &hit, &normal, &color, n, depth, context_hash);
}

double get_intersection(ray r, Octree* tree_node, vec* hit, vec* normal, vec* color, llist_node** object_node) {
    int i;
    double t, t_lowest;
    llist_node *n, *object_node_temp;
    vec hit_temp, normal_temp, color_temp;
    

    t = t_lowest = -1;
    // n = geometry->head;
    n = tree_node->primitives->head; // first level

    if(tree_node->octets != NULL) {
        for(i = 0; i < 8; i++) {
            if( ray_aabb_colides(r, tree_node) ) {
                t = get_intersection(r, &(tree_node->octets[i]), &hit_temp, &normal_temp, &color_temp, &object_node_temp);
                if(t > 0 && (t < t_lowest || t_lowest < 0)) {
                    t_lowest = t;

                    *hit            = hit_temp;
                    *normal         = normal_temp;
                    *color          = color_temp;
                    *object_node    = object_node_temp;
                }
            }
        }
    }
    while(n != NULL) {
        switch(n->payloadType) {
            case SPHERE:
                t = ray_sphere_get_intersection(r, ((Sphere*)n->payload), &hit_temp, &normal_temp, &color_temp);
                break;
            case PLANE:
                t = ray_plane_get_intersection(r, ((Plane*)n->payload), &hit_temp, &normal_temp, &color_temp);
                break;
            case TRIANGLE:
                t = ray_tri_get_intersection(r, ((Tri*)n->payload), &hit_temp, &normal_temp, &color_temp);
                break;
            default:
                t = -1;
                break;
        }
        if(t > 0 && (t < t_lowest || t_lowest < 0)) {
            t_lowest = t;

            *hit            = hit_temp;
            *normal         = normal_temp;
            *color          = color_temp;
            *object_node    = n;
        }
        
        n = n->next;
    }

    return t_lowest;
}

vec shade(Scene* scene, ray* r, vec* hit, vec* normal, vec* color, llist_node* object_node, int depth, long int* context_hash) {
    double shadow_t, light_t;
    long int dummy_context;
    vec amb, diffuse = {0,0,0}, specularity = {0,0,0}, reflection = {0,0,0}, ambient_occlusion = {1,1,1}, dummy;
    llist_node* n = scene->lights->head, *shadow_object;
    ray shadow_ray;
    Pointl* light;
    Material* m;

    *context_hash = (long int)object_node;
    *hit = vec_add(*hit, vec_mult(*normal, 0.00000001));
    m = get_material(object_node);

    while(n != NULL) {
        light = ((Pointl*)n->payload);
        shadow_ray = ray_create(*hit, vec_sub(light->pos,*hit));
        shadow_t = get_intersection(shadow_ray, &(scene->tree), &dummy, &dummy, &dummy, &shadow_object);
        light_t = vec_norm(vec_sub(light->pos,*hit));
        if(shadow_t > 0 && shadow_t < light_t) { // collision with object before light
            n = n->next;
            continue;
        }

        *context_hash += (long int)light;

        double dot_product = vec_dot(*normal, vec_unit(vec_sub(light->pos,*hit)));
        diffuse = vec_add(diffuse, vec_mult(vec_color_mult(*color, light->color),  MAX(0, dot_product)));

        specularity = vec_add(specularity, vec_mult(vec_add(vec_mult(*color, m->omega), vec_mult((vec){1,1,1}, (1-m->omega))), pow(MAX(0,vec_dot(vec_reflect(shadow_ray.direction, *normal), r->direction)),m->alpha)));

        n = n->next;
    }

    if(m->kr > 0)
        reflection = trace_ray(ray_create(*hit, vec_reflect(r->direction, *normal)), scene, depth+1, &dummy_context);


    amb         = vec_mult(vec_color_mult(*color, light->color), m->ka);
    diffuse     = vec_mult(diffuse, m->kd);
    specularity = vec_mult(specularity, m->ks);
    reflection  = vec_mult(reflection, m->kr);
    // ambient_occlusion = calc_ambient_occlusion(geometry, hit, normal);

    return vec_color_mult(ambient_occlusion, vec_add(vec_add(vec_add(diffuse, amb), specularity), reflection));
}

Material* get_material(llist_node* n) {
    switch(n->payloadType) {
        case SPHERE:
            return &((Sphere*)n->payload)->material;
        case PLANE:
            return &((Plane*)n->payload)->material;
        case TRIANGLE:
            return &((Tri*)n->payload)->material;
    }
    return NULL;
}

vec calc_ambient_occlusion(Scene* scene, vec* point, vec* normal) {
    int i;
    int hits = 0;
    vec dummy_vec;
    llist_node* dummy_node;
    ray sample;
    for(i = 0; i < AMBIENT_OCCLUSION_SAMPLES; i++) {
        sample = ray_create(*point, vec_unit((vec){rand(), rand(), rand()}));
        if(vec_dot(sample.direction, *normal) < 0)
            sample.direction = vec_mult(sample.direction, -1);
        if(get_intersection(sample, &(scene->tree), &dummy_vec, &dummy_vec, &dummy_vec, &dummy_node) > 0)
            hits++;
    }
    double shade = (double)(AMBIENT_OCCLUSION_SAMPLES-hits)/AMBIENT_OCCLUSION_SAMPLES;
    return (vec){shade, shade, shade};
}