// Raspicop@Jean Collas 2015-10-30 : config file, mainly hardware and clock configuration related
// Code based on richardghirst/PiBits demo_3d example
//   modified to object-oriented C++
//   remove internal MPU6050 reading
//   modified to allow external ypr values so that
//     the collecting data process is not dependent on the UI loop
//     it allows to update the UI at a lower speed, sparing the proc for more important things
//   removed offline option (not useful anymore as YPR can be defined outside the class)


#ifndef COPTER3D_H
#define COPTER3D_H
#include <gtkmm/drawingarea.h>
#include "geometry.h"

class Copter3D : public Gtk::DrawingArea
{
public:
    Copter3D();
    Copter3D(float *);
    bool setup();
    float * ypr;           // [yaw, pitch, roll]   yaw/pitch/roll container

protected:
    //Override default signal handler:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

    bool on_timeout();

    double m_line_width;
    double m_radius;

    double minx, maxx,  miny, maxy;
    double scale, xoff, yoff;
    object_t *objects;
    int nbObjects=0;

private:
    void addCube(point_t s, point_t r, point_t d);

    void calc_scale(const Cairo::RefPtr<Cairo::Context>& cr);

    static int get_quadrant(double *x, double *y);
    static void set_quadrant(double *x, double *y, int quad);
    static void transform(double *x, double *y, double rot);
    static void transform_x(point_t *p, int n, double rot);
    static void transform_y(point_t *p, int n, double rot);
    static void transform_z(point_t *p, int n, double rot);
    static void transform_o(point_t *p, int n, double o);



    void reset_scale(const Cairo::RefPtr<Cairo::Context>& cr);
    void accum_scale(const Cairo::RefPtr<Cairo::Context>& cr, point_t *p, int n);
    void cairo_move_to_scaled(const Cairo::RefPtr<Cairo::Context>& cr, point_t *p);
    void cairo_line_to_scaled(const Cairo::RefPtr<Cairo::Context>& cr, point_t *p);
    void draw_3d8(const Cairo::RefPtr<Cairo::Context>& cr, point_t *q);
};

#endif // COPTER3D_H
