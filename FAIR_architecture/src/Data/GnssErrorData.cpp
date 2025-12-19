/*
 * GnssErrorData.cpp
 *
 *  Created on: 19 December 2024
 *      Author: massimo
 */
 
#include "GnssErrorData.h"
#include <stdio.h>
 
GnssErrorData::GnssErrorData(string name):TimeStampedData(name)  {
	mPacketCode=GNSS_ERROR_CODE;
	reset();
}

GnssErrorData::GnssErrorData(void):TimeStampedData()  {
	mPacketCode=GNSS_ERROR_CODE;
	reset();
}

void GnssErrorData::reset(void) {
	time=0;
	rangeRms=0;
	stdMajor=0;
	stdMinor=0;
	orient=0;
	stdLat=0;
	stdLon=0;
	stdAlt=0;
}

GnssErrorData & GnssErrorData::operator = (const GnssErrorData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// time
	time=obj.time;
	// RMS value of the standard deviation of the ranges
	rangeRms=obj.rangeRms;
	// ellipse standard devation
	stdMajor=obj.stdMajor;
	stdMinor=obj.stdMinor;
	orient=obj.orient;
	// lat, lon, alt standard deviation
	stdLat=obj.stdLat;
	stdLon=obj.stdLon;
	stdAlt=obj.stdAlt;
	// return a reference to myself
	return *this;
}


void GnssErrorData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_time,"							// No CF
			"%s_rangeRMS,"						// No CF
			"%s_major_axis_stdev,"				// No CF
			"%s_minor_axis_stdev,"				// No CF
			"%s_major_axis_orientation,"		// No CF
			"%s_latitude_stdev,"				// No CF	(RF)
			"%s_longitude_stdev,"				// No CF	(RF)
			"%s_height_stdev,"					// No CF
			,strTS
			,mName.c_str()
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

void GnssErrorData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"gnss_time,"							// No CF
			"gnss_rangeRMS,"						// No CF
			"major_axis_stdev,"						// [m] No CF
			"minor_axis_stdev,"						// [m] No CF
			"major_axis_orientation,"				// [deg] No CF
			"latitude_stdev,"						// [m] No CF
			"longitude_stdev,"						// [m] No CF
			"height_stdev,"						// [m] No CF
			,strTS
			);
	strcpy(s,str);
}

int GnssErrorData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// time
			"%010.3lf "
			// RMS value of the standard deviation of the ranges
			"%.3lf "
			// ellipse standard devation
			"%.3lf %.3lf %.3lf "
			// lat, lon, alt standard deviation
			"%.3lf %.3lf %.3lf"
			" ",
			// time stamp
			str1,
			// time
			time,
			// RMS value of the standard deviation of the ranges
			rangeRms,
			// ellipse standard devation
			stdMajor,stdMinor,orient,
			// lat, lon, alt standard deviation
			stdLat,stdLon,stdAlt
			);
	strcpy(s,str);
	return n;
}


void GnssErrorData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// time
			"%010.3lf,"
			// RMS value of the standard deviation of the ranges
			"%.3lf,"
			// ellipse standard devation
			"%.3lf,%.3lf,%.3lf,"
			// lat, lon, alt standard deviation
			"%.3lf,%.3lf,%.3lf"
			",",
			// time stamp
			str1,
			// time
			time,
			// RMS value of the standard deviation of the ranges
			rangeRms,
			// ellipse standard devation
			stdMajor,stdMinor,orient,
			// lat, lon, alt standard deviation
			stdLat,stdLon,stdAlt
			);
	 strcpy(s,str);
}

void GnssErrorData::sread(char *s) {
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
			// time
			" %lf"
			// RMS value of the standard deviation of the ranges
			" %lf"
			// ellipse standard devation
			" %lf %lf %lf"
			// lat, lon, alt standard deviation
			" %lf %lf %lf"
			"",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// time
			&time,
			// RMS value of the standard deviation of the ranges
			&rangeRms,
			// ellipse standard devation
			&stdMajor,&stdMinor,&orient,
			// lat, lon, alt standard deviation
			&stdLat,&stdLon,&stdAlt
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

int GnssErrorData::sprint_NMEA_GTS(char *s,string talkerId) {
	string strNS,strEW;

	char packet[256];
	sprintf(packet,"$%sGTS,%09.2lf,%.3lf,%.3lf,%.3lf,%.1lf,%.3lf,%.3lf,%.3lf,*",
			talkerId.c_str(),time,rangeRms,stdMajor,stdMinor,orient,stdLat,stdLon,stdAlt);
	//
	// compute NMEA checksum
	//
//		mUint8Ptr=&(mDataPacketPtr->mData[0]);
	uint8_t* uint8Ptr = (uint8_t*) packet;
	uint8Ptr++;
	uint8_t NMEAchecksum=0;
	while(*uint8Ptr!='*') {
		NMEAchecksum ^= *uint8Ptr;
		uint8Ptr++;
	}

	int n=sprintf(packet,"$%sGTS,%09.2lf,%.3lf,%.3lf,%.3lf,%.1lf,%.3lf,%.3lf,%.3lf,*%x\r\n",
			talkerId.c_str(),time,rangeRms,stdMajor,stdMinor,orient,stdLat,stdLon,stdAlt,NMEAchecksum);

	return n;

}


