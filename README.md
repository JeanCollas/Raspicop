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

For the moment it is limited to the MPU control, some console interaction over SSH, some display console and 3D via GTK over SSH, that will be extended to radio-communication.
The control of ESCs will be implemented using the Servoblaster library of richardghirst.

# Howto

    make
    sudo ./Raspicop 2> errorOutput

# Content

## Main files and config
    config.h    // Hadware and loop config
    signals.h    
    worker.h    // Async worker
    main.cpp    // Main file
    main.h     
    Makefile    

## MPU6050 module
    helper_3dmath.h // Original 3DMath helper
    I2Cdev.h          // I2Cdev lib from jrowberg
    MPU6050_6Axis_MotionApps20.h // from jrowberg
    MPU6050.cpp                 // from jrowberg
    MPU6050.h                   // modified MPU6050 object
    MPU6050_Output.cpp          // MPU6050 object extension
    MPU6050_Output.h            // MPU6050 output object


## Console interaction and display modules
    inputs.h            // IO helpers functions
    helper_string.h     // string helper
    keyboard.cpp        // keyboard console module
    keyboard.h          // keyboard console module
    display.h           // console display module

## 3D Display
    geometry.h          // Objects definition
    copter3d.h          // 3D copter component
    window3d.h          // Window used to display the 3D copter
