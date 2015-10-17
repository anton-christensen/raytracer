#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "camera.h"
#include "common.h"
#include "llist.h"
#include "point_light.h"
#include "reader.h"
#include "vector.h"
#include "scene.h"

int main(int argc, char* argv[]) {
    int i,t;

    Scene scene;
    camera cam;
    Pointl* light;
    llist *geometry, *lights;


    if(argc < 2 || argc > 3) {
        printf("[USAGE] %s {file.ply} [{outfile.pnm}]\n", argv[0]);
        return 1;
    }
    setbuf(stdout, NULL); // disables stdout buffering
    t = time(NULL); // track time from this point on
    

    geometry = llist_create();
    lights   = llist_create();

    parse_ply(argv[1], geometry);

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

    vec eye = { 0, 0, 0},
         ul = {-.5, .5,-1},
         ur = { .5, .5,-1},
         ll = {-.5,-.5,-1},
         lr = { .5,-.5,-1};
    cam = camera_create(eye, ul, ur, ll, lr, COLS, ROWS);
    camera_auto_position(&cam, geometry);

    light = llist_add_new(lights, sizeof(Pointl), POINT);
    pointl_init(light, vec_add(cam.ul, (vec){0,0,1}), 1);

    scene_build(&scene, geometry, lights);

    camera_render(&cam, (argc == 3 ? argv[2] : "output.pnm"), &scene);


    printf("! RENDER_TIME %d\n", (int)time(NULL)-t);
    return 0;
}