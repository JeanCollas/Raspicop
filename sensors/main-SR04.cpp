#include <stdio.h>
#include <unistd.h>
#include "HC-SR04.h"

int main( int argc, const char* argv[] )
{
	// Prints each argument on the command line.
	for( int i = 0; i < argc; i++ )
	{
		printf( "arg %d: %s\n", i, argv[i] );
	}
	
	uint8_t pin_trig=23;
	uint8_t pin_echo_count=1;
	uint8_t pins_echo[1]={24};
	double distance;
	
	HC_SR04 sr;
	sr.Init(pin_trig, pin_echo_count, pins_echo);
//	sleep(1);
	for(int i=0;i<1;i++)
	{
//		nsleep(50); 
//		sleep(2);
		distance=sr.Measure();
		printf("Distance: %f\n", distance);
	}
	
//	sr.close_io();
	
//	printf("Distance: %f\n", distance);
	printf("Done\n");
	
	// while(1);
//	int x;
//	x = atoi( argv[1] );
	
	
}
