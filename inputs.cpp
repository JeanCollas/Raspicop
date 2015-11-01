// Raspicop@Jean Collas 2015-10-30 : several helper functions used for console manipulation input and output

#include "inputs.h"

namespace Inputs
{

	void ShowStdIn(int state)
	{
		if(state==NB_ENABLE)
		{
			ShowStdinKeystrokes();
			ShowConsoleCursor();
		}
		else
		{
			HideStdinKeystrokes();
			HideConsoleCursor();
		}
	}

	void HideConsoleCursor() 
	{
		printf("\e[?25l"); /* hide the cursor */
	}

	void ShowConsoleCursor() 
	{
		printf("\e[?25h"); /* show the cursor */
	}

	void HideStdinKeystrokes()
	{
		termios tty;

		tcgetattr(STDIN_FILENO, &tty);

		/* we want to disable echo */
		tty.c_lflag &= ~ECHO;

		tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	}

	void ShowStdinKeystrokes()
	{
	   termios tty;

		tcgetattr(STDIN_FILENO, &tty);

		/* we want to reenable echo */
		tty.c_lflag |= ECHO;

		tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	}

	int KbHit()
	{
		struct timeval tv;
		fd_set fds;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
		select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
		return FD_ISSET(STDIN_FILENO, &fds);
	}

	void NonBlock(int state)
	{
		struct termios ttystate;
	 
		//get the terminal state
		tcgetattr(STDIN_FILENO, &ttystate);
	 
		if (state==NB_ENABLE)
		{
			//turn off canonical mode
			ttystate.c_lflag &= ~ICANON;
			//minimum of number input read.
			ttystate.c_cc[VMIN] = 1;
		}
		else if (state==NB_DISABLE)
		{
			//turn on canonical mode
			ttystate.c_lflag |= ICANON;
		}
		//set the terminal attributes.
		tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
	 
	}

	/*
	int main()
	{
		char c;
		int i=0;
	 
		nonblock(NB_ENABLE);
		while(!i)
		{
			usleep(1);
			i=kbhit();
			if (i!=0)
			{
				c=fgetc(stdin);
				if (c=='q')
					i=1;
				else
					i=0;
			}
	 
			fprintf(stderr,"%d ",i);
		}
		printf("\n you hit %c. \n",c);
		nonblock(NB_DISABLE);
	 
		return 0;
	}

	*/

}
