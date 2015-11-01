// Raspicop@Jean Collas 2015-10-30 : definition of main loop interval and main objects

#include <stdio.h> // printf
#include <stdlib.h> // exit(0)

#include "helper_string.h"
#include "display.h"
#include "worker.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define LOOP_MICROSEC (20 * 1000)

// global objects used in the main loop
Keyboard keyboard;
Display disp(&keyboard);
MPU6050 mpu;
Worker * ui = NULL;


void stop_actions(int s);

void startUI();


#define LOOP_MICROSEC_ADJUSTMENT -82 // micro seconds to compensate additional delay due to non-interrupt

long getMax(long a, long b) {return a>b?a:b;}
long getMin(long a, long b) {return a>b?b:a;}

long getMissingMicrosec(timespec start);
