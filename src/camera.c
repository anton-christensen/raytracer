#include "camera.h"

camera camera_create(vec eye, vec ul, vec ur, vec ll, vec lr, int res_x, int res_y) {
	camera c = {eye, ul, ur, ll, lr, res_x, res_y};
	return c;
}

ray camera_get_ray(camera* cam, double index_x, double index_y) {
	vec p;
	double x = (double)index_x/cam->res_x;
	double y =1-(double)index_y/cam->res_y;
	
	p = vec_add(vec_mult(vec_add(vec_mult(cam->ll,1-y), vec_mult(cam->ul,y)),1-x), vec_mult(vec_add(vec_mult(cam->lr,1-y), vec_mult(cam->ur,y)),x));
	ray r = ray_create(cam->eye, vec_sub(p, cam->eye));
	return r;
}

void camera_translate(camera* cam, vec t) {
	cam->eye = vec_add(cam->eye, t);
	cam->ul = vec_add(cam->ul, t);
	cam->ur = vec_add(cam->ur, t);
	cam->ll = vec_add(cam->ll, t);
	cam->lr = vec_add(cam->lr, t);
}

void camera_render(camera* cam, char* outfile_path, llist* geometry, llist* lights) {
    FILE* f_img;
    ray r;
    vec color;

    long int context_x, last_context_x, last_context_y[COLS], dummy_context;
    int i,j;


    context_x = -2, last_context_x = -1;
    for(i = 0; i < COLS; i++) {
        last_context_y[i] = -1;
    }

	f_img = fopen(outfile_path, "wb");
    fputs("P6\n", f_img);
    fprintf(f_img, "%d %d\n",cam->res_x,cam->res_y);
    fputs("255\n", f_img);


    for(i = 0; i < cam->res_y ; i++) {
        for(j = 0; j < cam->res_x; j++) {
            int s_x, s_y;
            vec s_color = {0,0,0};

            r = camera_get_ray(cam, (double)j, (double)i);
            s_color = trace_ray(r, geometry, lights, 0, &context_x);
            if(!SMART_ANTIALIAS || context_x != last_context_x || context_x != last_context_y[j]) {
                for(s_y = 0; s_y < ANTIALIAS_FACTOR; s_y++) {
                    for(s_x = 1; s_x < ANTIALIAS_FACTOR; s_x++) {
                        r = camera_get_ray(cam, (double)j+(double)(s_x-ANTIALIAS_FACTOR/2)/ANTIALIAS_FACTOR, (double)i+(double)(s_y-ANTIALIAS_FACTOR/2)/ANTIALIAS_FACTOR);
                        s_color = vec_add(s_color, trace_ray(r, geometry, lights, 0, &dummy_context));
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
            printf("! PERCENTAGE %d\n", (int)((double)i/(double)ROWS*100.0));
    }

    fclose(f_img);
}

void camera_auto_position(camera* cam, llist* geometry) {
	vec size, center;
	calc_space_of_geometry(geometry, &size, &center);


    camera_translate(cam, center);
    double a = asin(0.5);
    camera_translate(cam, (vec){0,0,((size.y > size.x ? size.y : size.x) / tan(a)) });
}