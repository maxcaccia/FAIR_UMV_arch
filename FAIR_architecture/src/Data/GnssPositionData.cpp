/*
 * GnssPositionData.cpp
 *
 *  Created on: 16 November 2022
 *      Author: massimo
 */
 
#include "GnssPositionData.h"
#include <stdio.h>
 
GnssPositionData::GnssPositionData(string name):TimeStampedData(name)  {
	mPacketCode=GNSS_POSITION_CODE;
	reset();
}

GnssPositionData::GnssPositionData(void):TimeStampedData()  {
	mPacketCode=GNSS_POSITION_CODE;
	reset();
}

void GnssPositionData::reset(void) {
	date=0;
	time=0;
	latitude=0;
	longitude=0;
	heightAboveEllipsoid=0;
	heightAboveMSL=0;
	fixQuality=0;
	nSatellites=0;
	hdop=0;
}

GnssPositionData & GnssPositionData::operator = (const GnssPositionData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// date, time
	date=obj.date;
	time=obj.time;
	// latitude, longitude
	latitude=obj.latitude;
	longitude=obj.longitude;
	// GPS height
	heightAboveEllipsoid=obj.heightAboveEllipsoid;
	heightAboveMSL=obj.heightAboveMSL;
	// fix quality, n satellites, HDOP
	fixQuality=obj.fixQuality;
	nSatellites=obj.nSatellites;
	hdop=obj.hdop;
	// return a reference to myself
	return *this;
}


void GnssPositionData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_date,"							// No CF
			"%s_time,"							// No CF
			"%s_latitude,"						// CF
			"%s_longitude,"						// CF
			"%s_height_above_ellipsoid,"		// CF
			"%s_height_above_MSL,"				// CF
			"%s_fix_quality,"				// No CF	(RF)
			"%s_n_satellites,"				// No CF	(RF)
			"%s_HDOP,"		// No CF
			,strTS
			,mName.c_str()
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

void GnssPositionData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"gnss_date,"							// No CF
			"gnss_time,"							// No CF
			"latitude,"								// CF
			"longitude,"							// CF
			"height_above_reference_ellipsoid,"		// CF
			"height_above_mean_sea_level,"			// CF
			"gnss_quality,"							// No CF
			"gnss_number_of_satellites,"			// No CF
			"gnss_hdop,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int GnssPositionData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// date, time
			"%" PRIu64 " %010.3lf "
			// latitude, longitude
			"%.8lf %.8lf "
			// GPS height
			"%.3lf %.3lf "
			// fix quality, n satellites, HDOP
			"%" PRIu8 " %" PRIu8 " %.3lf"
			" ",
			// time stamp
			str1,
			// date and time
			date,time,
			// latitude, longitude
			latitude,longitude,
			// GPS height
			heightAboveEllipsoid,heightAboveMSL,
			// fix quality, n satellites, HDOP
			fixQuality,nSatellites,hdop
			);
	strcpy(s,str);
	return n;
}


void GnssPositionData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			 // date, time
			 "%" PRIu64 ",%010.3lf,"
			 // latitude, longitude
			 "%.8lf,%.8lf,"
			 // GPS height
			 "%.3lf,%.3lf,"
			 // fix quality, n satellites, HDOP
			 "%" PRIu8 ",%" PRIu8 ",%.3lf"
			 ",",
			 // time stamp
			 str1,
			 // date and time
			 date,time,
			 // latitude, longitude
			 latitude,longitude,
			 // GPS height
			 heightAboveEllipsoid,heightAboveMSL,
			 // fix quality, n satellites, HDOP
			 fixQuality,nSatellites,hdop
			 );
	 strcpy(s,str);
}

void GnssPositionData::sread(char *s) {
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
			&fixQuality,&nSatellites,&hdop
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

int GnssPositionData::sprint_NMEA_GGA(char *s,string talkerId) {
	string strNS,strEW;

	char packet[256];
	if(latitude>=0.) strNS="N";	else strNS="S";
	if(longitude>=0.) strEW="E"; else strEW="W";
	// sprint hhmmss.ss in UTC
	// sprint latitude [degmin.dddddddd]
	// sprint "N"/"S"
	// sprint longitude [degmin.dddddddd]
	// sprint "E"/"W"
	// sprint position fix indicator
	// sprint satellites used
	// sprint HDOP
	// sprint MSL altitude [m]
	// sprint "M"
	// sprint Geoid-to-ellipsoid separation. Ellipsoid altitude = MSL Altitude + Geoid Separation
	// sprint "M"
	// sprint Age of differential GPS data recor
	// sprint Diff. Ref. Station ID
	double latDegMin,lonDegMin;
	latDegMin=deg_dec_2_deg_min(latitude);
	lonDegMin=deg_dec_2_deg_min(longitude);

	sprintf(packet,"$%sGGA,%09.2lf,%011.6lf,%s,%012.6lf,%s,1,07,1.0,0.0,M,0.0,M,,*",
			talkerId.c_str(),time,fabs(latDegMin),strNS.c_str(),fabs(lonDegMin),strEW.c_str());

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

	int n=sprintf(s,"$%sGGA,%09.2lf,%011.6lf,%s,%012.6lf,%s,1,07,1.0,0.0,M,0.0,M,,*%x\r\n",
			talkerId.c_str(),time,fabs(latDegMin),strNS.c_str(),fabs(lonDegMin),strEW.c_str(),NMEAchecksum);
	return n;

}


int GnssPositionData::sprint_NMEA_ZDA(char *s,string talkerId) {
	char packet[256];
	// sprint hhmmss.sss in UTC
	// sprint dd
	// sprint mm
	// sprint yyyy
	// local time zone offset: hours
	// local time zone offset: minutes
	uint64_t yyyy,mm,dd;
	dd=date%100;
	yyyy=date/10000;
	mm=(date-(date/10000)*10000)/100;
	sprintf(packet,"$%sZDA,%010.3lf,%02d,%02d,%d,01,00*",
			talkerId.c_str(),time,(int)dd,(int)mm,(int)yyyy);

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

	int n=sprintf(s,"$%sZDA,%010.3lf,%02d,%02d,%d,01,00*%x\r\n",
			talkerId.c_str(),time,(int)dd,(int)mm,(int)yyyy,NMEAchecksum);
	return n;
}


double GnssPositionData::deg_dec_2_deg_min(double degDec) {
//printf("---------------- degDec %lf\n",degDec);
	double degDecAbs=fabs(degDec);
	double degDecSign;
	if(degDecAbs!=0)
		degDecSign=degDec/degDecAbs;
	else
		degDecSign=1.;
	double deg=floor(degDecAbs);
	double minDec=(degDecAbs-deg)*60.;
	double degMin=degDecSign*(100*deg+minDec);
	return degMin;
}
