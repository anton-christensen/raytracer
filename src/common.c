#include "common.h"

void calc_space_of_geometry(llist* geometry, vec* size, vec* position) {
    int i;
    double height_low, height_high;
    double width_low, width_high;
    double depth_low, depth_high;

    Plane* plane;
    Sphere* sphere;
    Tri* tri;

    height_low = 999999, height_high = -999999;
    width_low = 999999, width_high = -999999;
    depth_low = 999999, depth_high = -999999;

    llist_node* itt = geometry->head;
    while(itt != NULL) {
        switch(itt->payloadType) {
            case SPHERE:
                sphere = (Sphere*)itt->payload;
                if(sphere->pos.y-sphere->r < height_low)
                    height_low = sphere->pos.y-sphere->r;
                if(sphere->pos.y+sphere->r > height_high)
                    height_high = sphere->pos.y+sphere->r;
                if(sphere->pos.x-sphere->r < width_low)
                    width_low = sphere->pos.x-sphere->r;
                if(sphere->pos.x+sphere->r > width_high)
                    width_high = sphere->pos.x+sphere->r;
                if(sphere->pos.z-sphere->r < depth_low)
                    depth_low = sphere->pos.z-sphere->r;
                if(sphere->pos.z+sphere->r > depth_high)
                    depth_high = sphere->pos.z+sphere->r;
                break;
            case TRIANGLE:
                tri = (Tri*)itt->payload;
                for(i = 0; i < 3; i++) {
                    if(tri->vertex[i].y < height_low)
                        height_low = tri->vertex[i].y;
                    if(tri->vertex[i].y > height_high)
                        height_high = tri->vertex[i].y;
                    if(tri->vertex[i].x < width_low)
                        width_low = tri->vertex[i].x;
                    if(tri->vertex[i].x > width_high)
                        width_high = tri->vertex[i].x;
                    if(tri->vertex[i].z < depth_low)
                        depth_low = tri->vertex[i].z;
                    if(tri->vertex[i].z > depth_high)
                        depth_high = tri->vertex[i].z;
                }
        }

        itt = itt->next;
    }

    size->x = width_high - width_low;
    size->y = height_high - height_low;
    size->z = depth_high - depth_low;
    position->x = width_low + size->x/2;
    position->y = height_low + size->y/2;
    position->z = depth_low + size->z/2;
}