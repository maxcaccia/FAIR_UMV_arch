/*
 * ForceTorque.cpp
 *
 *  Created on: 17 December 2023
 *      Author: massimo
 */
 
#include "ForceTorque.h"
#include <stdio.h>
 
ForceTorque::ForceTorque(string name):TimeStampedData(name) {
	reset();
}

ForceTorque::ForceTorque(void):TimeStampedData() {
	reset();
}


void ForceTorque::reset(void) {
	X=0;
	Y=0;
	Z=0;
	K=0;
	M=0;
	N=0;
}

ForceTorque & ForceTorque::operator = (const ForceTorque &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// applied force to the vehicle rigid body
	X=obj.X;
	Y=obj.Y;
	Z=obj.Z;
	// applied torque to the vehicle rigid body
	K=obj.K;
	M=obj.M;
	N=obj.N;
	// return a reference to myself
	return *this;
}

void ForceTorque::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"%s_surge_force_fore,"
			"%s_sway_force_starboard,"
			"%s_heave_force_down,"
			"%s_roll_torque,"
			"%s_pitch_torque,"
			"%s_yaw_torque,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void ForceTorque::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"platform_surge_force_fore,"			//No CF	(RF)
			"platform_sway_force_starboard,"		//No CF	(RF)
			"platform_heave_force_down,"			//No CF	(RF)
			"platform_roll_torque,"					//No CF	(RF)
			"platform_pitch_torque,"				//No CF	(RF)
			"platform_yaw_torque,"					//No CF	(RF)
			,strTS
			);
	strcpy(s,str);
}


int ForceTorque::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"FORCE_TORQUE %s"
			// applied force to the vehicle rigid body
			"%.2lf %.2lf %.2lf "
			// applied torque to the vehicle rigid body
			"%.2lf %.2lf %.2lf"
			" ",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,Y,Z,
			// applied torque to the vehicle rigid body
			K,M,N // [N m]
			);
	strcpy(s,str);
	return n;
}


void ForceTorque::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"FORCE_TORQUE,%s"
			// applied force to the vehicle rigid body
			"%.2lf,%.2lf,%.2lf,"
			// applied torque to the vehicle rigid body
			"%.2lf,%.2lf,%.2lf"
			",",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,Y,Z,
			// applied torque to the vehicle rigid body
			K,M,N // [N m]
			);
	strcpy(s,str);
}

void ForceTorque::sread(char *s) {
	char str[64];
	char name[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			// applied force to the vehicle rigid body
			" %lf %lf %lf"
			// applied torque to the vehicle rigid body
			" %lf %lf %lf",
			str,
			// time stamp
			name,&mTimeStamp,
			// applied force to the vehicle rigid body
			&X,&Y,&Z,
			// applied torque to the vehicle rigid body
			&K,&M,&N // [N m]
			);
	mName=name;
//	strcpy(s,str);
}

