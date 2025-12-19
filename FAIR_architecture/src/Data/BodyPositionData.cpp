/*
 * BodyPositionData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "BodyPositionData.h"
#include <stdio.h>
 
BodyPositionData::BodyPositionData(string name):TimeStampedData(name)  {
	mPacketCode=BODY_POSITION_CODE;
	reset();
}

BodyPositionData::BodyPositionData(void):TimeStampedData()  {
	mPacketCode=BODY_POSITION_CODE;
	reset();
}

void BodyPositionData::reset(void) {
	lat=0.;
	lon=0.;
	latStd=0;
	lonStd=0;
}

BodyPositionData & BodyPositionData::operator = (const BodyPositionData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// latitude, longitude
	lat=obj.lat;
	lon=obj.lon;
	// latitude, longitude stdev
	latStd=obj.latStd;
	lonStd=obj.lonStd;
	// return a reference to myself
	return *this;
}


void BodyPositionData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_latitude,"						// CF
			"%s_longitude,"						// CF
			"%s_latitude_stdev,"							// No CF
			"%s_longitude_stdev,"							// No CF
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void BodyPositionData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"latitude,"								// CF
			"longitude,"							// CF
			"latitude_stdev,"							// No CF
			"longitude_stdev,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int BodyPositionData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// latitude, longitude
			"%.8lf %.8lf "
			// latitude, longitude stdev
			"%.2lf %.2lf "
			" ",
			// time stamp
			str1,
			 // latitude, longitude
			 lat,lon,
			// latitude, longitude stdev
			latStd,lonStd
			);
	strcpy(s,str);
	return n;
}


void BodyPositionData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			 // latitude, longitude
			 "%.8lf,%.8lf,"
			 // latitude, longitude stdev
			 "%.2lf,%.2lf"
			 ",",
			 // time stamp
			 str1,
			 // latitude, longitude
			 lat,lon,
			 // latitude, longitude stdev
			 latStd,lonStd
			 );
	 strcpy(s,str);
}

void BodyPositionData::sread(char *s) {
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
			// latitude,longitude stdev
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
			// latitude, longitude
			&lat,&lon,
			// latitude,longitude stdev
			&latStd,&lonStd
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void BodyPositionData::assign(GnssPositionData gnssPositionData)  {
	// latitude, longitude
	lat=gnssPositionData.latitude; // [deg.dddddddd] (GGA)
	lon=gnssPositionData.longitude; // [deg.dddddddd] (GGA)
	// latitude, longitude stdev
	compute_position_stdev(gnssPositionData);
}

bool BodyPositionData::compute_position_stdev(GnssPositionData gnssPositionData) {
	if(gnssPositionData.fixQuality == 1) {
		latStd = lonStd = GPS_FIX_STD;
		return true;
	}
	if(gnssPositionData.fixQuality == 2) {
		latStd = lonStd = DGNSS_STD;
		return true;
	}
	if(gnssPositionData.fixQuality == 4) {
		latStd = lonStd = RTK_FIXED_STD;
		return true;
	}
	if(gnssPositionData.fixQuality == 5) {
		latStd = lonStd = RTK_FLOAT_STD;
		return true;
	}
	return false;
}
