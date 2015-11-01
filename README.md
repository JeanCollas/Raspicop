# Raspicop
Implementation of a C++ Raspberry quadricopter

It is 'another' Picopter project, which still needs a lot of work to compete some other more advanced projects.
This project has been mainly done for fun, and started from scratch for educational purposes (Github, Raspberry, C++, sensors, ...).
It is composed of several modules, and is not finished yet, the application framework is done but the pilot is missing :)

I have started this repository to share my -very high- number of issues faced while implementing, hope this will help some people, and probably me in the future. I spent quite some hours on making everything work well, especially the 3D thing.
It aims at being improved. 


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

# Hardware

## Yet handled

    Raspberry Pi 2
    Wifi dongle
    MPU-6050 (Accelerometer, Gyroscope)
    
## To come

    ESCs (half way)
    HC-SR04 (Distance sensor), half way
    GY-264 HMC5883L (Compass)
    RFM12B (radio transmitters)
    or HC-12 SI4463 (wireless serial port)
    3G USB key (if ok with power consumption) + GPS for remote flights
    HC-SR501 (Infrared sensor)
    Camera + Infrared camera
    Battery checker
    
    Some Arduino delegation is useful
    
    
# Software
    Raspbian Jessie
    bcm2835 library
    gtkmm
    samba (to edit under windows)
    
    Windows + Putty + XLaunch
    
## To come
    a way to work I2C in user land (currently sudo)
    Servoblaster
    

# Content

## Main files and config
    config.h    // Hadware and loop config
    signals.h    
    worker.h    // Async worker
    main.cpp    // Main file
    main.h     
    Makefile    

## MPU6050 module
    helper_3dmath.h             // Original 3DMath helper
    I2Cdev.h                    // I2Cdev lib from jrowberg
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


# Specific issues encontered

## Username/password
First problem when starting the Pi: what is the default username/password?? Ok, after a little search it is pi/raspberry
Then how to set an "easy" password because we don't care about security: password has to be set via sudo: sudo su; passwd pi;

## Issue with the Wifi configuration, net access over wifi when the LAN is connected
    sudo apt-get remove ifplugd
    sudo route del default
    sudo route add default gw 192.168.1.1 # (wifi router IP)

## To find the Pi IP if assigned with dynamic DHCP sous windows (cmd), quick check all IPs of subnet:

    FOR /L %I IN (1,1,254) DO @echo %I & ping.exe -w 10 -n 1 192.168.1.%I | FIND /i "TTL"

## No X for sudo ssh applications

When running a GTK application over ssh, if the app is ran under sudo, it fails. 
Solution:
    Run script enableSudoDisplay.sh

What does it do: 
    xauth list
    sudo xauth add $
    where $ is the line corresponding to the X window.

## Setup Samba to edit the project via windows (quite easy)

    sudo apt-get install samba
    sudo smbpasswd -a pi
    sudo nano /etc/samba/smb.conf

then add something like this (Prog will be the name of the shared folder)

    [Prog]
    path = /home/pi/projects
    valid users = pi
    writable = yes

Under windows 10:

    Explorer: \\192.168.1.11 (any pi IP)
    click on a folder, in the ribbon: "rapid access" > Map to keep it as a network drive.
