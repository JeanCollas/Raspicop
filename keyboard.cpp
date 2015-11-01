// Raspicop@Jean Collas 2015-10-30 : keyboard handling function,
//   called at each loop, checks if keys have been pressed
//   pre-process arrows

#include "inputs.h"
#include "keyboard.h"
using namespace Inputs;

Keyboard::Keyboard()
{
	for(int i=0;i<4;i++)
		ArrowsLTRB[i]=0;

	KeyPressed=false;
	NonBlock(NB_ENABLE);
	ShowStdIn(NB_DISABLE);
}
Keyboard::~Keyboard()
{
	NonBlock(NB_DISABLE);
	ShowStdIn(NB_ENABLE);
}

bool Keyboard::LoopKeys()
{
	KeyPressed=0;
	int ch=0;
	char c;

    NonBlock(NB_ENABLE);
    ch = KbHit();
    if(ch!=0)
    {
        c=fgetc(stdin);
        KeyPressed=1;
        LastKeyType=KEYTYPE_STD;
        switch (c)
        {
            case 27:               // ESC char. Sent if arrow is pressed. If ESC is pressed it will block the input until 2 other keys are pressed
                c=fgetc(stdin);
                c=fgetc(stdin);
                switch(c)
                {
                    case 'D':
                        ArrowsLTRB[0]=1;
                        ArrowsLTRB[1]=0;
                        ArrowsLTRB[2]=0;
                        ArrowsLTRB[3]=0;
                        LastKeyType=KEYTYPE_ARROW;
                        break;
                    case 'A':
                        ArrowsLTRB[0]=0;
                        ArrowsLTRB[1]=1;
                        ArrowsLTRB[2]=0;
                        ArrowsLTRB[3]=0;
                        LastKeyType=KEYTYPE_ARROW;
                        break;
                    case 'C':
                        ArrowsLTRB[0]=0;
                        ArrowsLTRB[1]=0;
                        ArrowsLTRB[2]=1;
                        ArrowsLTRB[3]=0;
                        LastKeyType=KEYTYPE_ARROW;
                        break;
                    case 'B':
                        ArrowsLTRB[0]=0;
                        ArrowsLTRB[1]=0;
                        ArrowsLTRB[2]=0;
                        ArrowsLTRB[3]=1;
                        LastKeyType=KEYTYPE_ARROW;
                        break;

				}
				break;

			case 'q':
//				return false;
				break;
			// To be completed with special chars
		}
		LastKeyNumber=ch;
		LastKeyChar=c;
	}
	return true;
}

