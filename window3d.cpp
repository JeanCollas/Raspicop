// Raspicop@Jean Collas 2015-10-30 : Class used create the main 3D UI window
//   that contains the 3D object

#include <iostream>
#include <gtkmm.h>
#include "copter3d.h"
#include "worker.h"
#include "window3d.h"
using namespace std;

typedef void (*FuncPtr)();

/* test function for buttons
void run()
{
	std::cout << "Hey!" << std::endl;
}
*/

Window3D::Window3D (Glib::RefPtr<Gtk::Application> * appPtr, Copter3D* cop) : BT_Exit("Quit") {
	app=appPtr;
	Copter=cop;
	exitFunc = NULL;
	BT_Exit.signal_clicked().connect(sigc::mem_fun(*this, &Window3D::go));
	Background.set_orientation(Gtk::ORIENTATION_VERTICAL);

	Background.add(BT_Exit);
	Background.add(*Copter);

	add(Background);
	show_all();
}

void
Window3D::launchWindowYPR(float * yprRef)
{
    Glib::RefPtr<Gtk::Application> app =
       Gtk::Application::create(
         "org.raspicop.main");
    Copter3D cop(yprRef);
    Window3D::launchWindow(&app, &cop);
}

int
Window3D::launchWindow(Glib::RefPtr<Gtk::Application>* app, Copter3D* cop)
{
//        if(!Glib::thread_supported()) Glib::thread_init();
    Window3D win(app, cop);

//   win.set_default_size(900, 700);
    win.resize(900, 600);
    win.move(100,50);
    win.set_title("MPU6050 visualisation2");

    return (*app)->run(win);
}

void
Window3D::go () {
    if(exitFunc != NULL)
        return;
    hide();
    return;

    /* code to use non-concurrent action on button
    exitFunc = new Worker(&run);
    exitFunc->sig_done.connect(sigc::mem_fun(*this, &Window3D::exitFuncDone));
    exitFunc->start();

    BT_Exit.set_sensitive(false);
    BT_Exit.set_label("Working");
    */
}

void Window3D::exitFuncDone () {
    delete exitFunc;
    exitFunc = NULL;
    BT_Exit.set_sensitive(true);
    BT_Exit.set_label("Go!");
}

