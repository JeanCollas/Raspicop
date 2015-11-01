// Raspicop@Jean Collas 2015-10-30 : class handling the console output,
//   making easy to display data from the main loop
#ifndef DISPLAY_H
#define DISPLAY_H
#include <iostream>
#include <string>
#include <time.h>
#include "keyboard.h"
//using namespace std;


class Display 
{
	public:
		Display(Keyboard *kb);
		void Init();
		void LoopDisplay();
		void ClearScreen();
		~Display();
		std::string Line1; 
		std::string Line2;
		std::string Line3;
		std::string LineHelp;
		std::string LineError;
		
	private:
		Keyboard *keyboard;
		void GoUp();
		void Go1Up();
		void DisplayInfo();
		std::string _Text;
		timespec _lastDisplayTime;
		bool arrowsLTRB[4];
		int delay; // number of times passed in the loop
		timespec _lastDisplayTimeDelayed;
};
	
#endif
