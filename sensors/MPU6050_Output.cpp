// Raspicop@Jean Collas 2015-10-30 : Extension of MPU6050 original class
// to use the MPU6050 object as an independent object

#include <sstream> // string stream
#include <stdio.h> // printf

#include "helper_string.h"
#include "MPU6050.h"
#include "MPU6050_Output.h"


using namespace std;

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

int MPU6050::MPU_setup() {
	
	ErrorString="";
    
	// initialize device
    printf("Initializing I2C devices...\n");
    initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

    // load and configure the DMP
    printf("Initializing DMP...\n");
    devStatus = dmpInitialize();
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        printf("Enabling DMP...\n");
        setDMPEnabled(true);

        // enable Arduino interrupt detection
        //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        //attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        printf("DMP ready!\n");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = dmpGetFIFOPacketSize();
		
		return 0;
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        printf("DMP Initialization failed (code %d)\n", devStatus);
		ErrorString=string_format("DMP Initialization failed (code %d)", devStatus);
		return -1;
    }
}




int MPU6050::loopGetData()
{

	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer

	// if programming failed, don't try to do anything
    if (!dmpReady) return -1;
    // get current FIFO count
    fifoCount = getFIFOCount();

    if (fifoCount == 1024) {
        // reset so we can continue cleanly
        resetFIFO();
        ErrorString="FIFO overflow!";
		return -1;
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (fifoCount >= 42) {
        // read a packet from FIFO
        getFIFOBytes(fifoBuffer, packetSize);


            dmpGetQuaternion(&(Output.Quater), fifoBuffer);
            dmpGetAccel(&(Output.Acceleration), fifoBuffer);
            dmpGetEuler(Output.Euler, &(Output.Quater));
            dmpGetGravity(&(Output.GravityVector), &(Output.Quater));
            dmpGetYawPitchRoll(Output.YPR, &(Output.Quater), &(Output.GravityVector));
            dmpGetLinearAccel(&(Output.RealAcceleration), &(Output.Acceleration), &(Output.GravityVector));
            dmpGetLinearAccelInWorld(&(Output.WorldAcceleration), &(Output.RealAcceleration), &(Output.Quater));
    
        ErrorString="";
		return 0;
	}
	return -1;
}

string MPU6050::loopGetDataStr()
{
	std::ostringstream stringStream;
	stringStream << "\r";
			
	#ifdef OUTPUT_READABLE_QUATERNION
		// display quaternion values in easy matrix form: w x y z
		stringStream << string_format("quat %7.2f %7.2f %7.2f %7.2f    ", (Output.Quater).w,(Output.Quater).x,(Output.Quater).y,(Output.Quater).z);
	#endif
	#ifdef OUTPUT_READABLE_EULER
		// display Euler angles in degrees
		stringStream << string_format("euler %7.2f %7.2f %7.2f    ", Output.Euler[0] * 180/M_PI, Output.Euler[1] * 180/M_PI, Output.Euler[2] * 180/M_PI);
	#endif
	#ifdef OUTPUT_READABLE_YAWPITCHROLL
		// display Euler angles in degrees
		stringStream << string_format("ypr  %7.2f %7.2f %7.2f    ", Output.YPR[0] * 180/M_PI, Output.YPR[1] * 180/M_PI, Output.YPR[2] * 180/M_PI);
	#endif
	#ifdef OUTPUT_READABLE_REALACCEL
		// display real acceleration, adjusted to remove gravity
		stringStream << string_format("areal %6d %6d %6d    ", Output.RealAcceleration.x, Output.RealAcceleration.y, Output.RealAcceleration.z);
	#endif
	#ifdef OUTPUT_READABLE_WORLDACCEL
		// display initial world-frame acceleration, adjusted to remove gravity
		// and rotated based on known orientation from quaternion
		stringStream << string_format("aworld %6d %6d %6d    ", Output.WorldAcceleration.x, Output.WorldAcceleration.y, Output.WorldAcceleration.z);
	#endif

	std::string s = stringStream.str();

	return s;
}

void MPU6050::loopShowData()
{
	cout << loopGetDataStr() <<endl;
}

