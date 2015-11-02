// Raspicop@Jean Collas 2015-10-30 : config file, mainly hardware and clock configuration related
// Code based on richardghirst/PiBits demo_3d example
//   modified to object-oriented C++
//   remove internal MPU6050 reading
//   modified to allow external ypr values so that
//     the collecting data process is not dependent on the UI loop
//     it allows to update the UI at a lower speed, sparing the proc for more important things
//   removed offline option (not useful anymore as YPR can be defined outside the class)

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <iostream>

#include "copter3d.h"
#include "geometry.h"

using namespace std;

Copter3D::Copter3D(float * YPR) : Copter3D()
{
    ypr=YPR;
}

Copter3D::Copter3D() : m_line_width(0.05),  m_radius(0.42)
{

    objects=NULL;

    ypr=new float(3);
    ypr[0]=ypr[1]=ypr[2]=1;
    Glib::signal_timeout().connect( sigc::mem_fun(*this, &Copter3D::on_timeout), 40 );

    #ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
    //Connect the signal handler if it isn't already a virtual method override:
    signal_draw().connect(sigc::mem_fun(*this, &Copter3D::on_draw), false);
    #endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED

    addCube((point_t){2,2,0.5},(point_t){0,0,0},(point_t){0,0,0});
    addCube((point_t){0.5,0.5,1},(point_t){0,0,0},(point_t){0,0,0.75});

}



// This adds a cube of size (1,1,1) centered at (0,0,0), then it
// scales it up by (s) and moves it (d).  (r) is its current rotation
// around the axes, though it is ignored at present.
void
Copter3D::addCube(point_t s, point_t r, point_t d)
{
        int i;
        object_t *o = (object_t *)malloc(sizeof(object_t));
        memcpy(&o->r,&r,sizeof(r));
        o->n = 8;
        o->p = (point_t *)malloc(sizeof(point_t)*o->n);
        o->q = (point_t *)malloc(sizeof(point_t)*o->n);
        point_t c[] = {
                { +0.5, -0.5, +0.5 },
                { +0.5, +0.5, +0.5 },
                { -0.5, +0.5, +0.5 },
                { -0.5, -0.5, +0.5 },
                { +0.5, -0.5, -0.5 },
                { +0.5, +0.5, -0.5 },
                { -0.5, +0.5, -0.5 },
                { -0.5, -0.5, -0.5 }
        };
        for (i = 0; i < 8; i++) {
                o->p[i].x = c[i].x * s.x + d.x;
                o->p[i].y = c[i].y * s.y + d.y;
                o->p[i].z = c[i].z * s.z + d.z;
        }
        memcpy(o->q,o->p,sizeof(point_t)*o->n);
        o->next = objects;
        objects = o;
        nbObjects++;
}

bool
Copter3D::on_timeout()
{
    // force our program to redraw the entire window.
    Glib::RefPtr<Gdk::Window> win = get_window();

    object_t *o;

    o = objects;
    for (int i=0;i<nbObjects;i++) {
        memcpy(o->q,o->p,sizeof(point_t) * o->n);
        transform_x(o->q, 8, (ypr)[2]);
        transform_y(o->q, 8, (ypr)[1]);
        transform_z(o->q, 8, (ypr)[0]);
        transform_o(o->q, 8, 6);
        o = o->next;
    }

    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    return true;
}

int
Copter3D::get_quadrant(double *x, double *y)
{
        if (*y >= 0)
                return *x >= 0 ? 0 : 1;
        else
                return *x >= 0 ? 3 : 2;
}

void
Copter3D::set_quadrant(double *x, double *y, int quad)
{
        if (quad >= 2) {
                *y = -fabs(*y);
                if (quad == 2)
                        *x = -fabs(*x);
                else
                        *x = fabs(*x);
        } else {
                *y =  fabs(*y);
                if (quad == 1)
                        *x = -fabs(*x);
                else
                        *x = fabs(*x);
        }
}


void
Copter3D::transform(double *x, double *y, double rot)
{
        // Rotate round x axis, i.e. change y & z
        int quad = get_quadrant(x, y);
//g_print("transform: y %f, z %f, q %d\n",p->y,p->z,quad);
        double angle = atan(fabs(*y / *x));
        double hypot = sqrt(*y * *y + *x * *x);
        if (quad == 1 || quad == 3)
                angle = M_PI / 2 - angle;
        angle += quad * M_PI / 2;
//g_print("         : a %f, h %f\n", angle, hypot);
        angle += rot;
        while (angle < 0)
                angle += 2 * M_PI;
        while (angle >= 2 * M_PI)
                angle -= 2 * M_PI;
        quad = (int)(angle*2/M_PI);
        *x = hypot * cos(angle);
        *y = hypot * sin(angle);
        set_quadrant(x, y, quad);
//g_print("         : y %f, z %f, q %d\n",p->y,p->z,quad);
}

void
Copter3D::transform_x(point_t *p, int n, double rot)
{
        while (n--) {
                transform(&p->y, &p->z, rot);
                p++;
        }
}

void
Copter3D::transform_y(point_t *p, int n, double rot)
{
        while (n--) {
                transform(&p->x, &p->z, rot);
                p++;
        }
}

void
Copter3D::transform_z(point_t *p, int n, double rot)
{
        while (n--) {
                transform(&p->y, &p->x, rot);
                p++;
        }
}

void
Copter3D::transform_o(point_t *p, int n, double o)
{
        while (n--) {
                p->x += o;
                p++;
        }
}




bool Copter3D::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  object_t *o;

  // scale to unit square and translate (0, 0) to be (0.5, 0.5), i.e.
  // the center of the window
  cr->set_line_width(m_line_width);

  cr->save();
  cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);   // green
  cr->paint();
  cr->restore();

  double seconds= (ypr)[2];

  cr->save();
  cr->set_line_cap(Cairo::LINE_CAP_ROUND);

  cr->save();
  cr->set_line_width(3);
  cr->set_source_rgba(0.7, 0.7, 0.7, 0.8); // gray
  cr->move_to(0, 0);
  cr->line_to(sin(seconds) * (m_radius * 0.9),
    -cos(seconds) * (m_radius * 0.9));
  cr->stroke();
        reset_scale(cr);
        o = objects;
        for (int i=0;i<nbObjects;i++) {
                accum_scale(cr,o->q,o->n);
                o=o->next;
        }
        calc_scale(cr);
        o = objects;
        for (int i=0;i<nbObjects;i++) {
//        for (o = objects; o; o = o->next) {
                draw_3d8(cr,o->q);
                o=o->next;
        }
  cr->restore();

  return true;
}

void
Copter3D::calc_scale(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const double w = allocation.get_width() - 8;
  const double h = allocation.get_height() - 8;

        if (fabs(maxx - minx) > fabs(maxy - miny)) {
                scale = w / fabs(maxx - minx) * 0.7;
        } else {
                scale = h / fabs(maxy - miny)  * 0.7;
        }
        xoff = -minx;
        yoff = -maxy;
}





void
Copter3D::reset_scale(const Cairo::RefPtr<Cairo::Context>& cr)
{
    minx = miny = 1000000;
    maxx = maxy = -1000000;
//g_print("reset now %f - %f, %f - %f\n", minx,maxx,miny,maxy);
}


void
Copter3D::accum_scale(const Cairo::RefPtr<Cairo::Context>& cr, point_t *p, int n)
{
        double x,y;
        double vpd = 1;

        while (n--) {
                x = p->y * vpd / p->x;
                y = p->z * vpd / p->x;
                if (x < minx)
                        minx = x;
                if (x > maxx)
                        maxx = x;
                if (y < miny)
                        miny = y;
                if (y > maxy)
                        maxy = y;
//g_print("accum %f %f, now %f - %f, %f - %f\n", p->y,p->z,minx,maxx,miny,maxy);
                p++;
        }
}


void
Copter3D::cairo_move_to_scaled(const Cairo::RefPtr<Cairo::Context>& cr, point_t *p)
{
        double x, y;
        double vpd = 1;

        x = (p->y * vpd / p->x + xoff) * scale + 4;
        y = (p->z * vpd / p->x + yoff) * scale * -1 + 4;
//g_print("move_to(%f,%f)\n",x,y);
        cr->move_to(x, y);
}

void
Copter3D::cairo_line_to_scaled(const Cairo::RefPtr<Cairo::Context>& cr, point_t *p)
{
        double x, y;
        double vpd = 1;

        x = (p->y * vpd / p->x + xoff) * scale + 4;
        y = (p->z * vpd / p->x + yoff) * scale * -1 + 4;
//g_print("line_to(%f,%f)\n",x,y);
        //cairo_set_source_rgb (cr, c, 0, 0);
        cr->line_to(x, y);
}


void
Copter3D::draw_3d8(const Cairo::RefPtr<Cairo::Context>& cr, point_t *q)
{
        int c;
        static double col = 0.01;
        static double step = 0.001;

//	cairo_set_source_rgb (cr, col, 0, 0);
        cairo_move_to_scaled(cr, q+3);
        for (c = 0; c < 4; c++)
                cairo_line_to_scaled(cr, q+c);
        cairo_move_to_scaled(cr, q+7);
        for (c = 4; c < 8; c++)
                cairo_line_to_scaled(cr, q+c);
        for (c = 0; c < 4; c++) {
                cairo_move_to_scaled(cr, q+c);
                cairo_line_to_scaled(cr, q+c+4);
        }
        cr->stroke();
        col += step;
        if (col >= 1 || col <= 0)
                step = -step;
}

