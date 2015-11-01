// Raspicop@Jean Collas 2015-10-30 : main loop

#include <stdio.h> // printf
#include <stdlib.h> // exit(0)

#include "helper_string.h"
#include "display.h"
#include "signals.h"

#include "config.h"
#include "I2Cdev.h"
//#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"

#include "main.h"

#include "copter3d.h"
#include "worker.h"
#include "window3d.h"


using namespace std;

#define ERRORS_NUM 200


int main(int argc, char *argv[]) 
{
	register_SigActions(&stop_actions);
//	cout << sizeof(mpu)<<endl;
	mpu.MPU_setup();
	disp.Init();
	
	MPU_Output out;
	bool run=1;

	timespec startTime;
	timespec endTime;
	clock_gettime(CLOCK_MONOTONIC, &startTime);
	long error=0;
	long errors[ERRORS_NUM];
	int errorsDist[ERRORS_NUM];
	int errorNum=0;
	long avgError=0;
	long maxError=0;
	int maxErrorIdx=0;
	disp.LineHelp="space:clear screen, w:launch UI, q:quit";

	// removed to start 3D UI only on demand
	//	startUI();


	while(run==1)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);

		keyboard.LoopKeys();
		
		if(mpu.loopGetData()>=0)
		{
			disp.Line1=mpu.loopGetDataStr();
			out = mpu.Output;
		}
		else disp.LineError = mpu.ErrorString;
		disp.LoopDisplay();
		
		if(keyboard.KeyPressed==1 && keyboard.LastKeyType==KEYTYPE_STD)
			switch(keyboard.LastKeyChar)
			{
				case 'w':
					disp.Line3="UI starting!";
					startUI();
					break;
				case 'q':
					disp.Line3="Exit!";
					run=0;
					break;
				case 32:               // SPACE = clear screen
					disp.ClearScreen();
					break;
			}


		// /////////
		// Do not add code in the loop after this line,
		// it just quickly calculates interval error to check if some adjustments should be done
		// /////////
		usleep(getMissingMicrosec(startTime));
		clock_gettime(CLOCK_MONOTONIC, &endTime);

		error=(endTime.tv_nsec-startTime.tv_nsec)/1000;
		if(endTime.tv_sec - startTime.tv_sec > 0 )
		{
			// Supposed only 1 sec added
			error+=1000000;
		}

		errors[errorNum]=error-LOOP_MICROSEC;
		errorNum=(errorNum+1)%ERRORS_NUM;
		if(errorNum==0)
		{
			avgError=0;
			for(int i=0;i<ERRORS_NUM;i++)
				avgError+=errors[i];
			
			for(int i=0;i<ERRORS_NUM;i++)
				errorsDist[i]=abs(errors[i] - avgError);

			maxError=0;
			maxErrorIdx=0;
			for(int j=0;j<ERRORS_NUM/4;j++)
			{
				for(int i=0;i<ERRORS_NUM;i++)
					if(errorsDist[i]>maxError){maxErrorIdx=i; maxError=errorsDist[i];}
				errors[maxErrorIdx]=0;
				errorsDist[maxErrorIdx]=0;
			}
			
			avgError=0;
			for(int i=0;i<ERRORS_NUM;i++)
				avgError+=errors[i];
			
			avgError/=ERRORS_NUM;
			disp.Line2 = string_format("%6d ms error", avgError);
		}
	}
		
}

void startUI()
{
    if(ui==NULL) //Window3D::launchWindow
        ui=new Worker(Window3D::launchWindowYPR, mpu.Output.YPR);
    if(ui->Done==0)
    {
        disp.LineError="UI already started!";
        return;
    }
    (*ui).start();
}

void stop_actions(int s) {
  printf("Caught signal %d\n",s);
  exit(0);
}

long getMissingMicrosec(timespec start)
{
	long elapsedNsec, elapsedSec;
	timespec endTime;
	clock_gettime(CLOCK_MONOTONIC, &endTime);
	elapsedNsec = endTime.tv_nsec - start.tv_nsec;
	elapsedSec = endTime.tv_sec - start.tv_sec;
	if(elapsedSec>0 && elapsedNsec<0)
	{
		elapsedSec-=1;
		// supposed only 1 sec elapsed
		elapsedNsec+=1000000000;
	}

	// Supposed that loop is less than 1sec
	if(elapsedSec > 0) return 0;
//	if(elapsedSec > LOOP_MICROSEC/1000/1000) return 0;

	if(elapsedNsec/1000>LOOP_MICROSEC) return 0;
	return getMin(LOOP_MICROSEC, getMax(0, LOOP_MICROSEC-(elapsedNsec/1000) + LOOP_MICROSEC_ADJUSTMENT));
}

