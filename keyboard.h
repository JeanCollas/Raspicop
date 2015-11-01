// Raspicop@Jean Collas 2015-10-30 : keyboard handling function,
//   called at each loop, checks if keys have been pressed
//   pre-process arrows

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "inputs.h"

enum KeyType {KEYTYPE_STD, KEYTYPE_ARROW};

class Keyboard
{
public:
    Keyboard();
    bool LoopKeys();
    // this is reseted at each loop
    bool KeyPressed;
    // Indicates which is the arrow that has been pressed
    bool ArrowsLTRB[4];
    int LastKeyNumber;
    char LastKeyChar;
    KeyType LastKeyType;
    ~Keyboard();
};

#endif // KEYBOARD_H
