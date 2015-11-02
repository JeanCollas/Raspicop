#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h> // clock_gettime & time structs
#include <wiringPi.h>
#include "../wiringPiGPIO.h"
#include "time.h"
#include "../worker.h"

using namespace std;
using namespace Time;

//#define TRUE 1

#define TRIG WPI_PIN40_GPIO
#define ECHO WPI_PIN38_GPIO
//#define ECHO WPI_PIN36_GPIO

struct DistanceMeasure
{
    timespec MeasureTime;
    // distance in cm
    int Distance;
};

class HCSR04
{
public:
    // I tried to handle several modules by sending the signal
    // from 1 trigger pin and receiving from different echo pins,
    // however it seems that this leads to unreliable results.
    HCSR04(uint8_t trig, uint8_t echo);
    int measureDistanceSync();
    void measureDistanceAsync();
    void startMeasureDaemon();
    DistanceMeasure* getLastMeasure();
    int lastDistance;
    timespec lastReceivedMeasure;
    // minimum should be around 30 000us, 10us signal + ~7000/m.
    // Consider lower speed for longer measures
    long loopIntervalMicroSec;
    bool stop;
private:
    static void measureLoop(void *);
    timespec lastStartedMeasure;
    uint8_t _gpio_trig;
    uint8_t _gpio_echo;
    Worker * _worker;
};


HCSR04::HCSR04(uint8_t trig, uint8_t echo)
{
    loopIntervalMicroSec=30000;
    stop=false;
    _gpio_trig=trig;
    _gpio_echo=echo;

    wiringPiSetup();
    pinMode(_gpio_trig, OUTPUT);
    pinMode(_gpio_echo, INPUT);

    //TRIG pin must start LOW
    digitalWrite(_gpio_trig, LOW);
    delay(30);
}

int HCSR04::measureDistanceSync()
{
    //Send trig pulse
    digitalWrite(_gpio_trig, HIGH);
    delayMicroseconds(20);
    digitalWrite(_gpio_trig, LOW);

    //Wait for echo start
    while(digitalRead(_gpio_echo) == LOW);


    timespec startTimeTS, endTimeTS;
    clock_gettime(CLOCK_MONOTONIC, &startTimeTS);
    //Wait for echo end
    while(digitalRead(_gpio_echo) == HIGH);
    clock_gettime(CLOCK_MONOTONIC, &endTimeTS);

    long microSecTravel=(endTimeTS.tv_nsec - startTimeTS.tv_nsec)/1000;
    if(microSecTravel<0)microSecTravel+=1000000;

    //Get distance in cm, around 2500microsec for 40cm
    int distance = microSecTravel / 58.3;

    return distance;
}

void HCSR04::startMeasureDaemon()
{
    void* t=this;
    if(_worker==NULL)
        _worker=new Worker(HCSR04::measureLoop, t);
    if(_worker->Done==0)
    {
        return;
    }
    (*_worker).start();
}

void HCSR04::measureLoop(void* hcsr04)
{
	HCSR04 * o=(HCSR04 *)hcsr04;
	o->stop=false;
	timespec startTime;
	timespec endTime;
	int dist=0;
	while(!o->stop)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);

		dist = o->measureDistanceSync();

		usleep(getMissingMicrosec(startTime,&endTime, o->loopIntervalMicroSec));
		o->lastDistance=dist;
		o->lastReceivedMeasure.tv_nsec=endTime.tv_nsec;
		o->lastReceivedMeasure.tv_sec=endTime.tv_sec;
	}
}

DistanceMeasure* HCSR04::getLastMeasure()
{
    DistanceMeasure * dm=new DistanceMeasure;
    dm->MeasureTime.tv_sec=lastReceivedMeasure.tv_sec;
    dm->MeasureTime.tv_nsec=lastReceivedMeasure.tv_nsec;
    dm->Distance=lastDistance;
    return dm;
}

void setup() {
        wiringPiSetup();
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);

        //TRIG pin must start LOW
        digitalWrite(TRIG, LOW);
        delay(30);
}

int getCM() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);

        //Wait for echo start
        while(digitalRead(ECHO) == LOW);


		timespec startTimeTS, endTimeTS;
		clock_gettime(CLOCK_MONOTONIC, &startTimeTS);
		//Wait for echo end
		while(digitalRead(ECHO) == HIGH);
		clock_gettime(CLOCK_MONOTONIC, &endTimeTS);

        long microSecTravel=(endTimeTS.tv_nsec - startTimeTS.tv_nsec)/1000;
        if(microSecTravel<0)microSecTravel+=1000000;
//        long travelTime = micros() - startTime;
 //       cout << startTime<<" "<<travelTime<<endl;
 //       cout << startTime<<" "<<microSecTravel<<endl;


        //Get distance in cm, around 2500microsec for 40cm
        int distance = microSecTravel / 58.3;

        return distance;
}

int main(void) {
       // setup();

       // printf("Distance: %dcm\n", getCM());

    HCSR04 sr(TRIG, ECHO);
    printf("Distance: %dcm\n", sr.measureDistanceSync());

    sr.startMeasureDaemon();
    while(1)
    {
        printf("Distance: %dcm\n", sr.lastDistance);
        usleep(100000);
    }
        return 0;
}

/*
void testTimerPerfs()
{
    long endTime, microSecTravel;
    long startTime = micros();
    for(int i=0;i<100000;i++)
        endTime=micros();
    cout << "Micro: " << (endTime-startTime) << endl;

	timespec startTimeTS, endTimeTS;
	clock_gettime(CLOCK_MONOTONIC, &startTimeTS);
	for(int i=0;i<100000;i++)
		clock_gettime(CLOCK_MONOTONIC, &endTimeTS);

    long microSec=(endTimeTS.tv_nsec - startTimeTS.tv_nsec)/1000;
    if(microSecTravel<0)microSecTravel+=1000000;
    cout << "Clock: " << microSec << endl;
}
*/
