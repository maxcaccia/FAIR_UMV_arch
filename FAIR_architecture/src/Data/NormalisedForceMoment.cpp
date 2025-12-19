/*
 * NormalisedForceMoment.cpp
 *
 *  Created on: 25 March 2025
 *      Author: massimo
 */
 
#include "NormalisedForceMoment.h"
#include <stdio.h>
 
NormalisedForceMoment::NormalisedForceMoment(string name):TimeStampedData(name) {
	mPacketCode=NORMALISED_FORCE_MOMENT_CODE;
	reset();
}

NormalisedForceMoment::NormalisedForceMoment(void):TimeStampedData() {
	mPacketCode=NORMALISED_FORCE_MOMENT_CODE;
	reset();
}


void NormalisedForceMoment::reset(void) {
	X=0;
	N=0;
}

NormalisedForceMoment & NormalisedForceMoment::operator = (const NormalisedForceMoment &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// applied force to the vehicle rigid body
	X=obj.X;
	// applied moment to the vehicle rigid body
	N=obj.N;
	// return a reference to myself
	return *this;
}

void NormalisedForceMoment::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_normalised_surge_force_fore,"
			"%s_normalised_yaw_moment,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void NormalisedForceMoment::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_normalised_surge_force_fore,"			//No CF	(RF)
			"platform_normalised_yaw_moment_fore_starboard,"					//No CF	(RF)
			,strTS
			);
	strcpy(s,str);
}


int NormalisedForceMoment::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// applied force to the vehicle rigid body
			"%.2lf "
			// applied moment to the vehicle rigid body
			"%.2lf"
			" ",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,
			// applied moment to the vehicle rigid body
			N // [N m]
			);
	strcpy(s,str);
	return n;
}


void NormalisedForceMoment::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// applied force to the vehicle rigid body
			"%.2lf,"
			// applied moment to the vehicle rigid body
			"%.2lf"
			",",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,
			// applied moment to the vehicle rigid body
			N // [N m]
			);
	strcpy(s,str);
}

void NormalisedForceMoment::sread(char *s) {
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
			" %lf"
			// applied moment to the vehicle rigid body
			" %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// applied force to the vehicle rigid body
			&X,
			// applied moment to the vehicle rigid body
			&N // [N m]
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}

