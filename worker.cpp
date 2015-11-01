// Raspicop@Jean Collas 2015-10-30 : Class used to launch action functions in another thread

#include <gtkmm.h>
#include <iostream>
#include "worker.h"
#include "copter3d.h"

using namespace std;


Worker::Worker(void(*run) (void)) : thread(0), stop(false) {
    _run = run;
    arg=0;
    Done=1;
}

Worker::Worker(void(*run) (float *), float *argmt) : thread(0), stop(false) {
    _run1 = run;
    arg=1;
    arg1=argmt;
    Done=1;
}

// Called to start the processing on the thread, ensure only one call at a time
void Worker::start () {
    thread = Glib::Thread::create(sigc::mem_fun(*this, &Worker::run), true);
}

// When shutting down, we need to stop the thread
Worker::~Worker() {
  {
    Glib::Mutex::Lock lock (mutex);
    stop = true;
  }
  if (thread)
    thread->join(); // Here we block to truly wait for the thread to complete
}

void Worker::run () {

  while(true) {
    {
      Glib::Mutex::Lock lock (mutex);
      if (stop) break;
    }
    Done=0;
    if(arg == 0)
         _run();
    if(arg == 1)
        _run1(arg1);

    Done=1;
    cout << "Done!" << endl;
    sig_done();
    break;
  }
}
