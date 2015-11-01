// Raspicop@Jean Collas 2015-10-30 : Class used to launch action functions in another thread

#ifndef WORKER_H
#define WORKER_H
#include <gtkmm.h>
typedef void (*FuncPtr)();
typedef void (*FuncPtr1)(float *);

class Worker {
  public:
    // Constructor for the call of a function without args
    Worker(void(*run) (void));
    // Constructor for the call of a function without a float array as arg1
    Worker(void(*run) (float *), float *argmt);

    // Called to start the processing on the thread, ensure only one call at a time
    void start ();

    // When shutting down, we need to stop the thread
    ~Worker();
    Glib::Dispatcher sig_done;
    bool Done;

  protected:
    FuncPtr _run ;
    FuncPtr1 _run1 ;
    // This is where the real work happens
    void run ();
    Glib::Thread * thread;
    Glib::Mutex mutex;
    bool stop;
    int arg;
//    void * arg1;
    float * arg1;
};

#endif // WORKER_H
