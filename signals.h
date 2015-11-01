// Raspicop@Jean Collas 2015-10-30 : Class used to register the system interrupt signals
// Used to catch the CTRL + C signal and perform an action before exiting the program


#ifndef SIGNALS_H
#define SIGNALS_H
#include <signal.h> // sigaction

// Used to catch the CTRL + C signal and perform an action before exiting the program
// the action gets the signal number as a parameter.
void register_SigActions(void(*act) (int))
{
	  //handling of CTRL+C input
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = act;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

}

#endif
