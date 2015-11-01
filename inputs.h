// Raspicop@Jean Collas 2015-10-30 : several helper functions used for console manipulation input and output

#ifndef INPUTS_H
#define INPUTS_H

#include <sys/time.h>  
#include <sys/types.h> 
#include <sys/select.h> 
#include<stdio.h>
#include<unistd.h>
#include<termios.h>

#define NB_ENABLE 0 
#define NB_DISABLE 1

namespace Inputs
{


	int KbHit();
	void NonBlock(int state);

	void ShowStdIn(int state);


	void HideStdinKeystrokes();
	void ShowStdinKeystrokes();
	void HideConsoleCursor(); 
	void ShowConsoleCursor(); 

}


#endif
