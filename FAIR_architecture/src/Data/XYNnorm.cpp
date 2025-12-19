/*
 * XYNnorm.cpp
 *
 *  Created on: 21 July 2025
 *      Author: massimo
 */
 
#include "XYNnorm.h"
#include <stdio.h>
 
XYNnorm::XYNnorm(string name):TimeStampedData(name) {
	mPacketCode=XYN_NORM_CODE;
	reset();
}

XYNnorm::XYNnorm(void):TimeStampedData() {
	mPacketCode=XYN_NORM_CODE;
	reset();
}


void XYNnorm::reset(void) {
	X=0;
	Y=0;
	N=0;
}

XYNnorm & XYNnorm::operator = (const XYNnorm &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// applied force to the vehicle rigid body
	X=obj.X;
	Y=obj.Y;
	// applied moment to the vehicle rigid body
	N=obj.N;
	// return a reference to myself
	return *this;
}

void XYNnorm::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_percentage_surge_force_fore,"
			"%s_percentage_sway_force_starboard,"
			"%s_percentage_yaw_moment,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void XYNnorm::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_percentage_surge_force_fore,"			//No CF	(RF)
			"platform_percentage_sway_force_starboard,"		//No CF	(RF)
			"platform_percentage_yaw_moment_fore_starboard,"					//No CF	(RF)
			,strTS
			);
	strcpy(s,str);
}


int XYNnorm::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// applied force to the vehicle rigid body
			"%.2lf %.2lf "
			// applied moment to the vehicle rigid body
			"%.2lf"
			" ",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,Y,
			// applied moment to the vehicle rigid body
			N // [%]
			);
	strcpy(s,str);
	return n;
}


void XYNnorm::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// applied force to the vehicle rigid body
			"%.2lf,%.2lf,"
			// applied moment to the vehicle rigid body
			"%.2lf"
			",",
			// time stamp
			str1,
			// applied force to the vehicle rigid body
			X,Y,
			// applied moment to the vehicle rigid body
			N // [%]
			);
	strcpy(s,str);
}

void XYNnorm::sread(char *s) {
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
			" %lf %lf"
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
			&X,&Y,
			// applied moment to the vehicle rigid body
			&N // [%]
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}

