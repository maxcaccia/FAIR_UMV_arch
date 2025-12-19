/*
 * RangeModem.cpp
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
 
#include "RangeModem.h"
#include <stdio.h>
 
RangeModem::RangeModem(string name):TimeStampedData(name) {
	reset();
	mPacketCode=RANGE_MODEM_CODE;
}

RangeModem::RangeModem(void):TimeStampedData() {
	reset();
	mPacketCode=RANGE_MODEM_CODE;
}


void RangeModem::reset(void) {
	x=0;
	y=0;
	z=0;
	Pxx=0.;
	Pyy=0.;
	Pxy=0.;
	rng=0.;
}

RangeModem & RangeModem::operator = (const RangeModem &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// position in the localEarth-fixed NED frame
	x=obj.x;
	y=obj.y;
	z=obj.z;
	// estimated covariance of horizontal position
	Pxx=obj.Pxx;
	Pxx=obj.Pyy;
	Pxx=obj.Pxy;
	// range
	rng=obj.rng;
	// return a reference to myself
	return *this;
}

void RangeModem::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_x,"
			"%s_y,"
			"%s_z,"
			"%s_Pxx,"
			"%s_Pyy,"
			"%s_Pxy,"
			"%s_range,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void RangeModem::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"projection_x_coordinate,"
			"projection_y_coordinate,"
            "depth,"
			"covariance_over_projection_x_coordinate,"
			"covariance_over_projection_y_coordinate,"
            "crosscovariance_over_projection_x_y_coordinate,"
			"modem_range,"
            ,strTS
			);
	strcpy(s,str);
}


int LinearPosition::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// latitude, longitude
			"%.8lf %.8lf "
			// local Earth-fixed NED frame coordinates
			"%.8lf %.8lf "
			// position in the localEarth-fixed NED frame
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			x,y,z
			);
	strcpy(s,str);
	return n;
}


void LinearPosition::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// latitude, longitude
			"%.8lf,%.8lf,"
			// local Earth-fixed NED frame coordinates
			"%.8lf,%.8lf,"
			// position in the localEarth-fixed NED frame
			"%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			x,y,z
			);
	strcpy(s,str);
}

void LinearPosition::sread(char *s) {
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
			// latitude, longitude			
			" %lf %lf"
			// local Earth-fixed NED frame coordinates
			" %lf %lf"
			// position in the localEarth-fixed NED frame
			" %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// latitude, longitude
			&latitude,&longitude,
			// local Earth-fixed NED frame coordinates
			&lat0,&lon0,
			// position in the localEarth-fixed NED frame
			&x,&y,&z
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}


