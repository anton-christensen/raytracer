#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "reader.h"
#include "vector.h"
#include "camera.h"
#include "sphere.h"
#include "point_light.h"
#include "llist.h"
#include "common.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define ROWS 960
#define COLS 960

#define ANTIALIAS_FACTOR 1 // max 4
#define REFLECTION_DEPTH 1
#define AMBIENT_OCCLUSION_SAMPLES 1 // 128 samples
#define SMART_ANTIALIAS 0


camera cam;
ray r;

vec trace(ray r, llist* geometry, llist* lights, int depth, long int* context_hash);

Material* get_material(node* n) {
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

double get_intersection(ray r, llist* geometry, vec* hit, vec* normal, vec* color, node** object_node) {
    double t, t_lowest;
    node* n;
    vec hit_temp, normal_temp, color_temp;
    

    t = t_lowest = -1;
    n = geometry->head;

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

vec calc_ambient_occlusion(llist* geometry, vec* point, vec* normal) {
    int i;
    int hits = 0;
    vec dummy_vec;
    node* dummy_node;
    ray sample;
    for(i = 0; i < AMBIENT_OCCLUSION_SAMPLES; i++) {
        sample = ray_create(*point, vec_unit((vec){rand(), rand(), rand()}));
        if(vec_dot(sample.direction, *normal) < 0)
            sample.direction = vec_mult(sample.direction, -1);
        if(get_intersection(sample, geometry, &dummy_vec, &dummy_vec, &dummy_vec, &dummy_node) > 0)
            hits++;
    }
    double shade = (double)(AMBIENT_OCCLUSION_SAMPLES-hits)/AMBIENT_OCCLUSION_SAMPLES;
    return (vec){shade, shade, shade};
}

vec shade(llist* geometry, llist* lights, ray* r, vec* hit, vec* normal, vec* color, node* object_node, int depth, long int* context_hash) {
    double shadow_t, light_t;
    long int dummy_context;
    vec amb, diffuse = {0,0,0}, specularity = {0,0,0}, reflection = {0,0,0}, ambient_occlusion = {1,1,1}, dummy;
    node* n = lights->head, *shadow_object;
    ray shadow_ray;
    Pointl* light;
    Material* m;

    *context_hash = (long int)object_node;
    *hit = vec_add(*hit, vec_mult(*normal, 0.00000001));
    m = get_material(object_node);

    while(n != NULL) {
        light = ((Pointl*)n->payload);
        shadow_ray = ray_create(*hit, vec_sub(light->pos,*hit));
        shadow_t = get_intersection(shadow_ray, geometry, &dummy, &dummy, &dummy, &shadow_object);
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
        reflection = trace(ray_create(*hit, vec_reflect(r->direction, *normal)), geometry, lights, depth+1, &dummy_context);


    amb         = vec_mult(vec_color_mult(*color, light->color), m->ka);
    diffuse     = vec_mult(diffuse, m->kd);
    specularity = vec_mult(specularity, m->ks);
    reflection  = vec_mult(reflection, m->kr);
    // ambient_occlusion = calc_ambient_occlusion(geometry, hit, normal);

    return vec_color_mult(ambient_occlusion, vec_add(vec_add(vec_add(diffuse, amb), specularity), reflection));
}

vec trace(ray r, llist* geometry, llist* lights, int depth, long int* context_hash) {
    *context_hash = 0;

    if(depth > REFLECTION_DEPTH)
        return (vec){0,0,0};
    double t;
    vec hit, normal, color;
    node *n;
    
    t = get_intersection(r, geometry, &hit, &normal, &color, &n);

    if(t < 0) {
        return (vec){0.9,0.9,0.9};
    }

    return shade(geometry, lights, &r, &hit, &normal, &color, n, depth, context_hash);
}

int main(int argc, char* argv[]) {
    if(argc < 2 || argc > 3) {
        printf("[USAGE] %s {file.ply} [{outfile.pnm}]\n", argv[0]);
        return 1;
    }
    int t = time(NULL);
    
    setbuf(stdout, NULL); // disables stdout buffering

    int i,j;
    long int context_x, last_context_x, last_context_y[COLS], dummy_context;
    FILE* f_img;
    vec color;

    Plane* plane;
    Sphere* sphere;
    Pointl* light;
    Tri* tri;

    llist *geometry, *lights;
    
    context_x = -2, last_context_x = -1;
    for(i = 0; i < COLS; i++) {
        last_context_y[i] = -1;
    }

    geometry = llist_create();
    lights   = llist_create();

    parse_ply(argv[1], geometry);
    // return 0;

    // plane = llist_add_new(geometry, sizeof(Plane), PLANE);
    // plane_init(plane, (vec){0, -0.1, 0}, (vec){0,1,0}, mirror);
    // plane->color = (vec){1,1,1};

    // vec vert[3];
    // vert[0] = (vec){-1,-0.1,-3};
    // vert[1] = (vec){ 0, 1,  -5};
    // vert[2] = (vec){ 1,-0.1,-3};

    // tri = llist_add_new(geometry, sizeof(Tri), TRIANGLE);
    // tri_init(tri, vert, (vec){0,1,0}, metal);

    // sphere = llist_add_new(geometry, sizeof(Sphere), SPHERE);
    // sphere_init(sphere,  (vec){0,-.5,-4}, .5, (vec){0 ,0, 1}, mirror);
    // sphere = llist_add_new(geometry, sizeof(Sphere), SPHERE);
    // sphere_init(sphere,  (vec){-1,0,0}, 0.1, (vec){1 ,0, 0}, diffuse);

    /// CALCULATE CAMERA POSITION
    vec m_center;
    double m_height, m_width, m_depth;
    double m_height_low = 999999, m_height_high = -999999;
    double m_width_low = 999999, m_width_high = -999999;
    double m_depth_low = 999999, m_depth_high = -999999;

    node* itt = geometry->head;
    while(itt != NULL) {
        switch(itt->payloadType) {
            case SPHERE:
                sphere = (Sphere*)itt->payload;
                if(sphere->pos.y-sphere->r < m_height_low)
                    m_height_low = sphere->pos.y-sphere->r;
                if(sphere->pos.y+sphere->r > m_height_high)
                    m_height_high = sphere->pos.y+sphere->r;
                if(sphere->pos.x-sphere->r < m_width_low)
                    m_width_low = sphere->pos.x-sphere->r;
                if(sphere->pos.x+sphere->r > m_width_high)
                    m_width_high = sphere->pos.x+sphere->r;
                if(sphere->pos.z-sphere->r < m_depth_low)
                    m_depth_low = sphere->pos.z-sphere->r;
                if(sphere->pos.z+sphere->r > m_depth_high)
                    m_depth_high = sphere->pos.z+sphere->r;
                break;
            case TRIANGLE:
                tri = (Tri*)itt->payload;
                for(i = 0; i < 3; i++) {
                    if(tri->vertex[i].y < m_height_low)
                        m_height_low = tri->vertex[i].y;
                    if(tri->vertex[i].y > m_height_high)
                        m_height_high = tri->vertex[i].y;
                    if(tri->vertex[i].x < m_width_low)
                        m_width_low = tri->vertex[i].x;
                    if(tri->vertex[i].x > m_width_high)
                        m_width_high = tri->vertex[i].x;
                    if(tri->vertex[i].z < m_depth_low)
                        m_depth_low = tri->vertex[i].z;
                    if(tri->vertex[i].z > m_depth_high)
                        m_depth_high = tri->vertex[i].z;
                }
        }

        itt = itt->next;
    }

    m_height = m_height_high - m_height_low;
    m_width = m_width_high - m_width_low;
    m_depth = m_depth_high - m_depth_low;
    m_center.x = m_width_low + m_width/2;
    m_center.y = m_height_low + m_height/2;
    m_center.z = m_depth_low + m_depth/2;
    // printf("model height: %lf\nmodel width: %lf\nmodel depth: %lf\n", m_height, m_width, m_depth);

    vec eye = { 0, 0, 0},
         ul = {-.5, .5,-1},
         ur = { .5, .5,-1},
         ll = {-.5,-.5,-1},
         lr = { .5,-.5,-1};

    cam = camera_create(eye, ul, ur, ll, lr, COLS, ROWS);
    camera_translate(&cam, m_center);
    double a = asin(0.5);
    camera_translate(&cam, (vec){0,0,((m_height > m_width ? m_height : m_width) / tan(a)) });


    ///

    light = llist_add_new(lights, sizeof(Pointl), POINT);
    pointl_init(light, cam.ul, 1);

    f_img = fopen((argc == 3 ? argv[2] : "output.pnm"), "wb");
    fputs("P6\n", f_img);
    fprintf(f_img, "%d %d\n",COLS,ROWS);
    fputs("255\n", f_img);

    for(i = 0; i < ROWS ; i++) {
        for(j = 0; j < COLS; j++) {
            int s_x, s_y;
            vec s_color = {0,0,0};

            r = camera_get_ray(cam, (double)j, (double)i);
            s_color = trace(r, geometry, lights, 0, &context_x);
            if(!SMART_ANTIALIAS || context_x != last_context_x || context_x != last_context_y[j]) {
                for(s_y = 0; s_y < ANTIALIAS_FACTOR; s_y++) {
                    for(s_x = 1; s_x < ANTIALIAS_FACTOR; s_x++) {
                        r = camera_get_ray(cam, (double)j+(double)(s_x-ANTIALIAS_FACTOR/2)/ANTIALIAS_FACTOR, (double)i+(double)(s_y-ANTIALIAS_FACTOR/2)/ANTIALIAS_FACTOR);
                        s_color = vec_add(s_color, trace(r, geometry, lights, 0, &dummy_context));
                    }
                }
                color = vec_devide(s_color, ANTIALIAS_FACTOR*ANTIALIAS_FACTOR);
            }
            else
                color = s_color;
            last_context_x = context_x;
            last_context_y[j] = context_x;
            fputc(MAX(0.0,MIN(1.0, color.x))*255, f_img);
            fputc(MAX(0.0,MIN(1.0, color.y))*255, f_img);
            fputc(MAX(0.0,MIN(1.0, color.z))*255, f_img);
        }
        if(i % (ROWS/50) == 0 )
            printf("%d%% ", (int)((double)i/(double)ROWS*100.0));
    }

    fclose(f_img);
    printf("\nFinished in %d seconds\n", (int)time(NULL)-t);

    return 0;
}