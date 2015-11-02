// Raspicop@Jean Collas 2015-10-30 : class used for 3D display
//  code extracted from richardghirst/PiBits demo_3d example

#ifndef GEOMETRY
#define GEOMETRY

typedef struct _point {
        double x,y,z;
} point_t;

typedef struct _object {
        struct _object *next;
        int n;
        point_t *p, *q;
        point_t r;
} object_t;


#endif // GEOMETRY

