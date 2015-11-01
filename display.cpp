// Raspicop@Jean Collas 2015-10-30 : class handling the console output,
//   making easy to display data from the main loop

#include <time.h>
#include "display.h"
#include "inputs.h"
#include "keyboard.h"
using namespace std;
using namespace Inputs;

Display::Display(Keyboard *kb)
{
	keyboard=kb;
}

void Display::Init()
{
	for(int i=0;i<20;i++)
		cout << endl;
	
	Line1="";
	Line2="";
	Line3="";
	LineHelp="";
	LineError="";
}

Display::~Display()
{
	cout << endl << endl  << endl;
}

void Display::ClearScreen()
{
	cout << "\033[2J" << flush;
}

void Display::GoUp()
{
	cout << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A";
	cout << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A";
	cout << "\x1b[A" << "\x1b[A" 
		<< "\x1b[A" 
		<< "\x1b[A"
		<< "\x1b[A"
		<< "\x1b[A"
		<< "\x1b[A";
//	cout << "\x1b[A";
}
void Display::Go1Up()
{
	cout << "\x1b[A" ;
	
}

void Display::DisplayInfo()
{
	
	
	timespec time;
	long elapsedNsec=0;
	long elapsedSec=0;

	delay = (delay + 1) % 2;
	
	cout << endl; 

	
	cout << "\t" << "   " << arrowsLTRB[1] << endl;
	cout << "\t" << arrowsLTRB[0]<< "     " << arrowsLTRB[2] << endl;
	cout << "\t" << "   " << arrowsLTRB[3] << endl;
	
	cout << endl ;

	cout << endl ;
	cout << endl ;
	cout << endl ;
	cout << endl ;
	cout << endl ;
	
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
	// elapsedNsec = time.tv_nsec - _lastDisplayTime.tv_nsec;
	// elapsedSec = time.tv_sec - _lastDisplayTime.tv_sec;
	// _lastDisplayTime = time;
	// cout << elapsedNsec/1000 << "ms" << endl;
	if(delay == 0)
	{
		clock_gettime(CLOCK_MONOTONIC, &time);
		elapsedNsec = time.tv_nsec - _lastDisplayTimeDelayed.tv_nsec;
		elapsedSec = time.tv_sec - _lastDisplayTimeDelayed.tv_sec;
		if(elapsedSec>0 && elapsedNsec<0)
		{
			elapsedSec-=1; 
			elapsedNsec+=1000000000;
		}
		_lastDisplayTimeDelayed = time;
		cout <<"000   "<< "000   " << "000   " << "000   "
			<< "\r"    << "      " << "      " << "      " << (elapsedNsec/2)%1000 << "ns" 
			<< "\r"    << "      " << "      " << (elapsedNsec/1000/2)%1000 << "us"
			<< "\r"    << "      " << (elapsedNsec/1000/1000/2)%1000 << "ms"
			<< "\r"    << elapsedSec << "s " 
			<< endl
			<< Line1 << endl
			<< Line2 << endl
			<< Line3 << endl
			<< LineHelp << endl
			<< LineError << endl
			<< endl;
			
	} else cout 
			<< endl << endl 
			<< endl
			<< endl
			<< endl
			<< endl << endl;
	
}


void Display::LoopDisplay()
{
	GoUp();
	
	if((*keyboard).KeyPressed==1)
	{
		Go1Up();
		
		cout << "\r" << _Text << "Key " << (*keyboard).LastKeyNumber << " pressed. " << (int)(*keyboard).LastKeyChar << " " <<endl;
		
		_Text="";
		if((*keyboard).LastKeyType == KEYTYPE_ARROW)
		{
			for(int i=0;i<4;i++)
				if((*keyboard).ArrowsLTRB[i]==1)
					arrowsLTRB[i]=1-arrowsLTRB[i];
		}
	}
	
	DisplayInfo();
}

