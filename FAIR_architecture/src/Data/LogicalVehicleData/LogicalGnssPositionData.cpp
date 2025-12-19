/*
 * LogicalGnssPositionData.cpp
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
 
#include "LogicalGnssPositionData.h"
#include <stdio.h>
 
LogicalGnssPositionData::LogicalGnssPositionData(string name):GnssPositionData(name)  {
	mPacketCode=LOGICAL_GNSS_POSITION_CODE;
	reset();
}

LogicalGnssPositionData::LogicalGnssPositionData(void):GnssPositionData()  {
	mPacketCode=LOGICAL_GNSS_POSITION_CODE;
	reset();
}

void LogicalGnssPositionData::reset(void) {
	GnssPositionData::reset();
	latStd=0;
	lonStd=0;
}

LogicalGnssPositionData & LogicalGnssPositionData::operator = (const LogicalGnssPositionData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	GnssPositionData::operator =(obj);
	// latitude, longitude
	latStd=obj.latStd;
	lonStd=obj.lonStd;
	// return a reference to myself
	return *this;
}


void LogicalGnssPositionData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	GnssPositionData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_latitude_stdev,"							// No CF
			"%s_longitude_stdev,"							// No CF
			,strTS
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void LogicalGnssPositionData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	GnssPositionData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"latitude_stdev,"							// No CF
			"longitude_stdev,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int LogicalGnssPositionData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	GnssPositionData::sprint(str1);
	n=sprintf(str,"%s"
			// latitude, longitude stdev
			"%.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// latitude, longitude stdev
			latStd,lonStd
			);
	strcpy(s,str);
	return n;
}


void LogicalGnssPositionData::sprint_csv(char *s) {
	char str[2048],str1[256];

	GnssPositionData::sprint_csv(str1);
	sprintf(str,"%s"
			 // latitude, longitude
			 "%.2lf,%.2lf"
			 ",",
			 // time stamp
			 str1,
			 // latitude, longitude stdev
			 latStd,lonStd
			 );
	 strcpy(s,str);
}

void LogicalGnssPositionData::sread(char *s) {
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
			// date, time
			" %" SCNu64 " %lf"
			// latitude, longitude
			" %lf %lf"
			// GPS height
			" %lf %lf"
			// fix quality, n satellites, HDOP
			" %" SCNu8 " %" SCNu8 " %lf"
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
			// date and time
			&date,&time,
			// latitude, longitude
			&latitude,&longitude,
			// GPS height
			&heightAboveEllipsoid,&heightAboveMSL,
			// fix quality, n satellites, HDOP
			&fixQuality,&nSatellites,&hdop,
			// latitude,longitude stdev
			&latStd,&lonStd
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalGnssPositionData::assign(GnssPositionData gnssPositionData)  {
	// date, time
	date=gnssPositionData.date; // yyyymmdd : date = 10000*yyyy + 100*mm + dd (ZDA, obtained by...)
	time=gnssPositionData.time; // hhmmss.ss in UTC (GGA, ZDA)
	// latitude, longitude
	latitude=gnssPositionData.latitude; // [deg.dddddddd] (GGA)
	longitude=gnssPositionData.longitude; // [deg.dddddddd] (GGA)
	// GPS height
	heightAboveEllipsoid=gnssPositionData.heightAboveEllipsoid; // [m] (GGA: Ellipsoid altitude = MSL Altitude + Geoid Separation)
	heightAboveMSL=gnssPositionData.heightAboveMSL; // [m] (GGA)
	// fix quality, n satellites, HDOP
	fixQuality=gnssPositionData.fixQuality; // (GGA)
	nSatellites=gnssPositionData.nSatellites; // GGA)
	hdop=gnssPositionData.hdop; // (GGA)
}
