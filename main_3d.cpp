// Raspicop@Jean Collas 2015-10-30 : main loop to test 3D UI independently

#include "copter3d.h"
#include "window3d.h"

using namespace std;

int main (int argc , char ** argv) {
    if(!Glib::thread_supported()) Glib::thread_init();
    Glib::RefPtr<Gtk::Application> app =
       Gtk::Application::create(
         "org.raspicop.tests");

    Copter3D cop;
    Window3D::launchWindow(&app, &cop);
    return 0;
    Gtk::Window window;

    window.set_default_size(400, 200);
    window.set_title("Tutorial 1");

    return (app)->run(window);
}

