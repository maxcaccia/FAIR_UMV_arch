/*
 * LogicalGnssVelocityData.cpp
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
 
#include "LogicalGnssVelocityData.h"
#include <stdio.h>
 
LogicalGnssVelocityData::LogicalGnssVelocityData(string name):GnssVelocityData(name)  {
	mPacketCode=LOGICAL_GNSS_VELOCITY_CODE;
	reset();
}

LogicalGnssVelocityData::LogicalGnssVelocityData(void):GnssVelocityData()  {
	mPacketCode=LOGICAL_GNSS_VELOCITY_CODE;
	reset();
}

void LogicalGnssVelocityData::reset(void) {
	GnssVelocityData::reset();
	speedStd=0;
	trackStd=0;
}

LogicalGnssVelocityData & LogicalGnssVelocityData::operator = (const LogicalGnssVelocityData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	GnssVelocityData::operator =(obj);
	// speed, track
	speedStd=obj.speedStd;
	trackStd=obj.trackStd;
	// return a reference to myself
	return *this;
}


void LogicalGnssVelocityData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	GnssVelocityData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_speed_stdev,"							// No CF
			"%s_course_stdev,"							// No CF
			,strTS
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void LogicalGnssVelocityData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	GnssVelocityData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_speed_wrt_ground_stdev,"							// No CF
			"platform_course_stdev,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int LogicalGnssVelocityData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	GnssVelocityData::sprint(str1);
	n=sprintf(str,"%s"
			// speed, track stdev
			"%.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// speed, track stdev
			speedStd,trackStd
			);
	strcpy(s,str);
	return n;
}


void LogicalGnssVelocityData::sprint_csv(char *s) {
	char str[2048],str1[256];

	GnssVelocityData::sprint_csv(str1);
	sprintf(str,"%s"
			 // speed, track
			 "%.2lf,%.2lf"
			 ",",
			 // time stamp
			 str1,
			 // speed, track stdev
			 speedStd,trackStd
			 );
	 strcpy(s,str);
}

void LogicalGnssVelocityData::sread(char *s) {
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

void LogicalGnssVelocityData::assign(GnssVelocityData gnssVelocityData)  {
	// speed, track
	speed=gnssVelocityData.speed; // [m/s]
	track=gnssVelocityData.track; // [deg]
}
