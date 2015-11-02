// Raspicop@Jean Collas 2015-10-30 : Extension of MPU6050 original class
// to use the MPU6050 object as an independent object
// Definition of the MPU_Output struct for the MPU6050 class,
// so that the MPU can be ran outside the main loop

#ifndef _MPU_6050_OUTPUT_H_
#define _MPU_6050_OUTPUT_H_


#include "helper_3dmath.h"


struct MPU_Output {
	Quaternion Quater;             // [w, x, y, z]         quaternion container
	VectorInt16 Acceleration;      // [x, y, z]            accel sensor measurements
	VectorInt16 RealAcceleration;  // [x, y, z]            gravity-free accel sensor measurements
	VectorInt16 WorldAcceleration; // [x, y, z]            world-frame accel sensor measurements
	VectorFloat GravityVector;     // [x, y, z]            gravity vector
	float Euler[3];                // [psi, theta, phi]    Euler angle container
	float YPR[3];                  // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
	
};


#endif
