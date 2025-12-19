/*
 * SeaCurrent.cpp
 *
 *  Created on: 14 February 2025
 *      Author: massimo
 */
 
#include "SeaCurrent.h"
#include <stdio.h>
 
SeaCurrent::SeaCurrent(string name):TimeStampedData(name) {
	mPacketCode=SEA_CURRENT_CODE;
	reset();
}

SeaCurrent::SeaCurrent(void):TimeStampedData() {
	mPacketCode=SEA_CURRENT_CODE;
	reset();
}


void SeaCurrent::reset(void) {
	xCdot=0;
	yCdot=0;
}

SeaCurrent & SeaCurrent::operator = (const SeaCurrent &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// sea current
	xCdot=obj.xCdot;
	yCdot=obj.yCdot;
	// return a reference to myself
	return *this;
}

void SeaCurrent::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_sea_current_North,"
			"%s_sea_current_East,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void SeaCurrent::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
            "northward_sea_water_velocity,"			// CF
            "eastward_sea_water_velocity,"			// CF
			,strTS
			);
	strcpy(s,str);
}


int SeaCurrent::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// sea current
			"%.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// sea current
			xCdot,yCdot
			);
	strcpy(s,str);
	return n;
}


void SeaCurrent::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// sea current
			"%.2lf,%.2lf,",
			// time stamp
			str1,
			// sea current
			xCdot,yCdot
			);
	strcpy(s,str);
}

void SeaCurrent::sread(char *s) {
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
			// sea current
			" %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// sea current
			&xCdot,&yCdot
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}


