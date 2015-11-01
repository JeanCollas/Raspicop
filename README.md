# Raspicop
Implementation of a C++ Raspberry quadricopter

This project has been mainly done for fun, and started from scratch for educational purposes (Github, Raspberry, C++, sensors, ...).
It is composed of several modules, and is not finished yet, the application framework is done but the pilot is missing :)

More to come!

\o/ My first Github repository filled!

# History
I started this project by buying a Raspberry Pi 2, some sensors, ESC and motors. 
More details to come...

# What this code does

For the moment it is limited to the MPU control, some display and console interaction over SSH, that will be extended to radio-communication.
The control of ESCs will be implemented using the Servoblaster library of richardghirst.

# Howto

    make
    sudo ./Raspicop 2> errorOutput
