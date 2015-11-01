// Raspicop@Jean Collas 2015-10-30 : Class used create the main 3D UI window
//   that contains the 3D object

#include <iostream>
#include <gtkmm.h>
#include "copter3d.h"
#include "worker.h"

class Window3D : public Gtk::Window
{
public:
    Window3D (Glib::RefPtr<Gtk::Application> * appPtr, Copter3D* cop) ;
    void go () ;
    void exitFuncDone () ;
    static int launchWindow(Glib::RefPtr<Gtk::Application>* appPtr, Copter3D* cop);
    static void launchWindowYPR(float * yprRef);
private:
    Glib::RefPtr<Gtk::Application> * app;
    Gtk::Paned Background;
    Gtk::Button BT_Exit;
    Gtk::Grid Grid3D;
    Copter3D* Copter;
    Worker * exitFunc;

};
