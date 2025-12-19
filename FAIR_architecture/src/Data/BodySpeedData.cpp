/*
 * BodySpeedData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "BodySpeedData.h"
#include <stdio.h>
 
BodySpeedData::BodySpeedData(string name):TimeStampedData(name)  {
	mPacketCode=BODY_SPEED_CODE;
	reset();
}

BodySpeedData::BodySpeedData(void):TimeStampedData()  {
	mPacketCode=BODY_SPEED_CODE;
	reset();
}

void BodySpeedData::reset(void) {
	speed=0.;
	track=0.;
	speedStd=0;
	trackStd=0;
}

BodySpeedData & BodySpeedData::operator = (const BodySpeedData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// speed, track
	speed=obj.speed;
	track=obj.track;
	// speed, track stdev
	speedStd=obj.speedStd;
	trackStd=obj.trackStd;
	// return a reference to myself
	return *this;
}


void BodySpeedData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_speed,"							// No CF	(RF)
			"%s_course,"							// No CF	(RF)
			"%s_speed_stdev,"							// No CF
			"%s_course_stdev,"							// No CF
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void BodySpeedData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_speed_wrt_ground,"	// CF
			"platform_course,"				// CF
			"platform_speed_wrt_ground_stdev,"							// No CF
			"platform_course_stdev,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int BodySpeedData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// speed, track
			"%.2lf %.2lf "
			// speed, track stdev
			"%.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// speed, track
			speed,track,
			// speed, track stdev
			speedStd,trackStd
			);
	strcpy(s,str);
	return n;
}


void BodySpeedData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			 // speed, track
			 "%.2lf,%.2lf,"
			 // speed, track
			 "%.2lf,%.2lf"
			 ",",
			 // time stamp
			 str1,
			 // speed, track
			 speed,track,
			 // speed, track stdev
			 speedStd,trackStd
			 );
	 strcpy(s,str);
}

void BodySpeedData::sread(char *s) {
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
			// speed, track
			" %lf %lf"
			// speed,track stdev
			" %lf %lf"
			"",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// speed, track
			&speed,&track,
			// speed,track stdev
			&speedStd,&trackStd
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void BodySpeedData::assign(GnssVelocityData gnssVelocityData)  {
	// speed, track
	speed=gnssVelocityData.speed; // [m/s]
	track=gnssVelocityData.track; // [deg]
}
