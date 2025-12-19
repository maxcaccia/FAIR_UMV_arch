/*
 * GnssVelocityData.cpp
 *
 *  Created on: 16 November 2022
 *      Author: massimo
 */
 
#include "GnssVelocityData.h"
#include <stdio.h>
 
GnssVelocityData::GnssVelocityData(string name):TimeStampedData(name)  {
	mPacketCode=GNSS_VELOCITY_CODE;
	reset();
}

GnssVelocityData::GnssVelocityData(void):TimeStampedData()  {
	mPacketCode=GNSS_VELOCITY_CODE;
	reset();
}

void GnssVelocityData::reset(void) {
	speed=0;
	track=0;
}

GnssVelocityData & GnssVelocityData::operator = (const GnssVelocityData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// speed and track
	speed=obj.speed;
	track=obj.track;
	// return a reference to myself
	return *this;
}


void GnssVelocityData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_speed,"							// No CF	(RF)
			"%s_course,"							// No CF	(RF)
			,strTS
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void GnssVelocityData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_speed_wrt_ground,"	// CF
			"platform_course,"				// CF
			,strTS
			);
	strcpy(s,str);
}

int GnssVelocityData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// speed and track
			"%.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// speed and track
			speed,track
			);
	strcpy(s,str);
	return n;
}


void GnssVelocityData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			 // speed and track
			 "%.2lf,%.2lf"
			 ",",
			 // time stamp
			 str1,
			 // speed and track
			 speed,track
			 );
	 strcpy(s,str);
}

void GnssVelocityData::sread(char *s) {
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
			// speed and track
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
			// speed and track
			&speed,&track
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}


int GnssVelocityData::sprint_NMEA_VTG(char *s,string talkerId) {
	char packet[256];
	// sprint track [deg] - True
	// sprint "T"
	// sprint track [deg] - magnetic
	// sprint "M"
	// sprint speed [knots]
	// sprint "N"
	// sprint speed [km/h]
	// sprint "K"
	// sprint mode indicator A/D/E/M/S/N
	double speedKnots=speed/0.514444;
	double speedKmh=speed*3.6;
	sprintf(packet,"$%sVTG,%.2lf,T,,M,%.2lf,N,%.2lf,K,D*",
			talkerId.c_str(),mod360(track),speedKnots,speedKmh);

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

	int n=sprintf(s,"$%sVTG,%.2lf,T,,M,%.2lf,N,%.2lf,K,D*%x\r\n",
			talkerId.c_str(),mod360(track),speedKnots,speedKmh,NMEAchecksum);
	return n;
}

