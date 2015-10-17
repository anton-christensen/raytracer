#include "scene.h"

void scene_build(Scene* scene, llist* geometry, llist* lights) {

    scene->lights = lights;

    // 1: calculate root cube from border values of model/scene
    calc_space_of_geometry(geometry, &scene->tree.size, &scene->tree.center);

    // 2: add all polygons to root
    scene->tree.primitives = geometry;
    
    scene_build_octree(&scene->tree, scene->tree.size, scene->tree.center);
}

void scene_build_octree(Octree* node, vec dimensions, vec center) {
    int i, j;
    char in_octet = -1;
    char in_n_octets = 0;
    llist_node* itt;
    vec sub_dimensions, sub_center[8];
    Tri* tri;

    for(i = 0; i < 8; i++)

    // allocate space for sub_nodes
    node->octets = malloc(sizeof(Octree)*8);
    for(i = 0; i < 8; i++) {
        get_subnode_pos_and_size(dimensions, center, i, &node->octets[i].size, &node->octets[i].center);   
        node->octets[i].octets = NULL;
        node->octets[i].primitives = llist_create();
    }

    // 3: For all polygons in node
    itt = node->primitives->head;
    while(itt != NULL) {
        switch(itt->payloadType) {
            case TRIANGLE:
                tri = (Tri*)itt->payload;
                // for each verticie while not registered in more than one octet
                for(i = 0; i < 3 && in_n_octets < 2; i++) {
                    // 4:   for each octant that we havn't registered as being inside, 
                    //        check if primitive is inside
                    for(j = 0; j < 8 && in_octet != j; j++) {
                        if(tri->vertex[i].x >= sub_center[j].x - sub_dimensions.x/2 &&
                           tri->vertex[i].x <= sub_center[j].x + sub_dimensions.x/2 &&
                           tri->vertex[i].y >= sub_center[j].y - sub_dimensions.y/2 &&
                           tri->vertex[i].y <= sub_center[j].y + sub_dimensions.y/2 &&
                           tri->vertex[i].z >= sub_center[j].z - sub_dimensions.z/2 &&
                           tri->vertex[i].z <= sub_center[j].z + sub_dimensions.z/2 ) {
                            in_octet = j;
                            in_n_octets++;
                        } 
                    }
                }
                break;
        }
        // 5:   if primitive is contained inside only one octet
        if(in_n_octets == 1) {
            // 6:     add primitive to that octets primitive list
            llist_remove_node(node->primitives, itt);
            llist_push_node(node->octets[(int)in_octet].primitives, itt);
        }
        itt = itt->next;
    }

    // 7: For all octets in node 
    for(i = 0; i<8; i++) {
        // 8:   if octet has more than one polygon
        if(node->octets[i].primitives->length > 1) {
            // 9:     goto step 3  
            scene_build_octree(&node->octets[i], node->octets[i].size, node->octets[i].center);
        }
    }
}

void get_subnode_pos_and_size(vec parrent_dimensions, vec parrent_center, int index, vec* dimensions, vec* center) {
    // index zyx
    // binar 000 // low end of xyz
    // binar 101 // high end z and x, low y
    *dimensions = vec_devide(parrent_dimensions, 2);
    *center = parrent_center; 
    if((index & 1<<0) == 0) // low x
        center->x -= dimensions->x;
    if((index & 1<<0) == 1) // high x
        center->x += dimensions->x;
    if((index & 1<<1) == 0) // low y
        center->y -= dimensions->y;
    if((index & 1<<1) == 1) // high y
        center->y += dimensions->y;
    if((index & 1<<2) == 0) // low z
        center->z -= dimensions->z;
    if((index & 1<<2) == 1) // high z
        center->z += dimensions->z;
}
