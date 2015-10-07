#ifndef _MATERIAL_
#define _MATERIAL_

typedef struct {
	double ka; // ambient  coefficient
	double kd; // diffuse  coefficient
	double ks; // specular coefficient
	double alpha; // specular exponent/shinyness
    double omega; // metalness
    double kr;  // reflective coefficient
} Material;

Material mirror;
Material metal;
Material shiny;
Material diffuse;
Material ambient;

#endif