#include "reader.h"

void parse_ply(char* filename, llist* geometry) {
    char str[256];
    FILE* fp;
    int nverticies = 0;
    int nfaces = 0, nfaces_per_polygon;
    int i, j;
    double x,y,z;
    unsigned int* vertex_list;
    vec *verticies, *colors;
    vec vert[3];
    Tri* tri;
    char has_color = 0;


    fp = fopen(filename, "r");
    fgets(str, 256, fp);
    if(strcmp(str, "ply\n") != 0)
        printf("NOT PLY\n");

    // while(fgets(str, 256, fp) != NULL && strcmp(str,"end_header\n") != 0) {
    while(fscanf(fp, "%s", str) != EOF && strcmp(str,"end_header") != 0) {
        if(strcmp(str, "element") == 0 && fscanf(fp, "%s", str) != EOF) {
            if(strcmp(str, "vertex") == 0) {
                fscanf(fp, "%u", &nverticies);
            } else if(strcmp(str, "face") == 0) {
                fscanf(fp, "%u", &nfaces);
            }
        }
        if(strcmp(str, "comment") == 0) { fgets(str, 256, fp);}
        if(strcmp(str, "property") == 0) { 
            fscanf(fp, "%*s %s", str);
            if(strcmp(str, "red") == 0 || strcmp(str, "green") == 0 || strcmp(str, "blue") == 0) {
                has_color = 1;
            } 
        }

    }

    printf("! POLYGONS %d\n", nfaces);
    verticies = (vec*)malloc(sizeof(vec)*nverticies);
    if(has_color)
        colors = (vec*)malloc(sizeof(vec)*nverticies);

    for(i = 0; i < nverticies; i++) {
        fscanf(fp, "%lf %lf %lf\n", &x, &y, &z);
        verticies[i] = (vec){x,y,z};
        if(has_color) {
            fscanf(fp, "%lf %lf %lf\n", &x, &y, &z);
            colors[i] = (vec){x,y,z};
        }
    }
    for(i = 0; i < nfaces; i++) {
        fscanf(fp, "%u", &nfaces_per_polygon);
        vertex_list = (unsigned int*)malloc(sizeof(unsigned int)*nfaces_per_polygon);
        for(j = 0; j < nfaces_per_polygon; j++) {
            fscanf(fp, "%u", &vertex_list[j]);
        }
        for(j = 0; j < nfaces_per_polygon-2; j++) {
            vert[0] = verticies[vertex_list[0]];
            vert[1] = verticies[vertex_list[j+1]];
            vert[2] = verticies[vertex_list[j+2]];

            if(has_color) {
                vec c = vec_add(colors[vertex_list[0]], vec_add(colors[vertex_list[j+1]], colors[vertex_list[j+2]]));
                tri = llist_add_new(geometry, sizeof(Tri), TRIANGLE);
                tri_init(tri, vert, c, shiny);
            }
            else {
                tri = llist_add_new(geometry, sizeof(Tri), TRIANGLE);
                tri_init(tri, vert, (vec){1.0,0,0}, shiny);
            }

        }

    }
    fclose(fp);
}