#include "reader.h"

void parse_ply(char* filename, llist* geometry) {
    char str[256];
    FILE* fp;
    int nverticies = 0;
    int nfaces = 0;
    int i;
    double x,y,z;
    unsigned int a,b,c;
    vec* verticies;
    vec vert[3];
    Tri* tri;


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

    }

    verticies = (vec*)malloc(sizeof(vec)*nverticies);

    for(i = 0; i < nverticies; i++) {
        fscanf(fp, "%lf %lf %lf\n", &x, &y, &z);
        verticies[i] = (vec){x,y,z};
    }
    for(i = 0; i < nfaces; i++) {
        fscanf(fp, "%*u %u %u %u\n", &a, &b, &c);

        vert[0] = verticies[a];
        vert[1] = verticies[b];
        vert[2] = verticies[c];

        tri = llist_add_new(geometry, sizeof(Tri), TRIANGLE);
        tri_init(tri, vert, (vec){0,1.0,0}, shiny);

    }
    fclose(fp);
}