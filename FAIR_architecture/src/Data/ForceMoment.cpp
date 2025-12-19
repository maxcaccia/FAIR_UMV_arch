/*
 * ForceTorque.cpp
 *
 *  Created on: 17 December 2023
 *      Author: massimo
 */
 
#include "ForceMoment.h"
#include <stdio.h>
 
ForceMoment::ForceMoment(string name):TimeStampedData(name) {
	mPacketCode=FORCE_MOMENT_CODE;
	reset();
}

ForceMoment::ForceMoment(void):TimeStampedData() {
	mPacketCode=FORCE_MOMENT_CODE;
	reset();
}


void ForceMoment::reset(void) {
	X=0;
	Y=0;
	Z=0;
	K=0;
	M=0;
	N=0;
}

ForceMoment & ForceMoment::operator = (const ForceMoment &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// applied force to the vehicle rigid body
	X=obj.X;
	Y=obj.Y;
	Z=obj.Z;
	// applied moment to the vehicle rigid body
	K=obj.K;
	M=obj.M;
	N=obj.N;
	// return a reference to myself
	return *this;
}

void ForceMoment::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_surge_force_fore,"
			"%s_sway_force_starboard,"
			"%s_heave_force_down,"
			"%s_roll_moment,"
			"%s_pitch_moment,"
			"%s_yaw_moment,"
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


void ForceMoment::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_surge_force_fore,"			//No CF	(RF)
			"platform_sway_force_starboard,"		//No CF	(RF)
			"platform_heave_force_down,"			//No CF	(RF)
			"platform_roll_moment_starboard_down,"					//No CF	(RF)
			"platform_pitch_moment_fore_up,"				//No CF	(RF)
			"platform_yaw_moment_fore_starboard,"					//No CF	(RF)
			,strTS
			);
	strcpy(s,str);
}


int ForceMoment::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// applied force to the vehicle rigid body
			"%.2lf %.2lf %.2lf "
			// applied moment to the vehicle rigid body
			"%.2lf %.2lf %.2lf"
			" ",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,Y,Z,
			// applied moment to the vehicle rigid body
			K,M,N // [N m]
			);
	strcpy(s,str);
	return n;
}


void ForceMoment::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// applied force to the vehicle rigid body
			"%.2lf,%.2lf,%.2lf,"
			// applied moment to the vehicle rigid body
			"%.2lf,%.2lf,%.2lf"
			",",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,Y,Z,
			// applied moment to the vehicle rigid body
			K,M,N // [N m]
			);
	strcpy(s,str);
}

void ForceMoment::sread(char *s) {
	char packetCode[64];
	char name[64];
	uint8_t valid;
	sscanf(s,// packet_code
			"%s "
			// name
			"%s "
			// time stamp
			"%lf "
			// valid flag
			" %" SCNu8
			// applied force to the vehicle rigid body
			" %lf %lf %lf"
			// applied moment to the vehicle rigid body
			" %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// applied force to the vehicle rigid body
			&X,&Y,&Z,
			// applied moment to the vehicle rigid body
			&K,&M,&N // [N m]
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}

