/*
 * GPSdata.cpp
 *
 *  Created on: 16 November 2022
 *      Author: massimo
 */
 
#include "GPSdata.h"
#include <stdio.h>
 
GPSdata::GPSdata(string name):TimeStampedData(name)  {
	 reset();
}

GPSdata::GPSdata(void):TimeStampedData()  {
	 reset();
}

void GPSdata::reset(void) {
	date=0;
	time=0;
	latitude=0;
	longitude=0;
	heightAboveEllipsoid=0;
	heightAboveMSL=0;
	speed=0;
	track=0;
	fixQuality=0;
	nSatellites=0;
	hdop=0;
}

GPSdata & GPSdata::operator = (const GPSdata &obj) {
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
	// speed and track
	speed=obj.speed;
	track=obj.track;
	// fix quality, n satellites, HDOP
	fixQuality=obj.fixQuality;
	nSatellites=obj.nSatellites;
	hdop=obj.hdop;
	// return a reference to myself
	return *this;
}


void GPSdata::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"%s_date,"							// No CF
			"%s_time,"							// No CF
			"%s_latitude,"						// CF
			"%s_longitude,"						// CF
			"%s_height_above_ellipsoid,"		// CF
			"%s_height_above_MSL,"				// CF
			"%s_speed,"							// No CF	(RF)
			"%s_track,"							// No CF	(RF)
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
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void GPSdata::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"packet_code,%s"
			"date,"									// No CF
			"time,"									// No CF
			"latitude,"								// CF
			"longitude,"							// CF
			"height_above_reference_ellipsoid,"		// CF
			"height_above_mean_sea_level,"			// CF
			"platform_speed,"						// No CF	(RF)
			"platform_track,"						// No CF	(RF)
			","										// No CF
			","										// No CF
			","										// No CF
			,strTS
			);
	strcpy(s,str);
}

int GPSdata::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"GPS %s "
			// date, time
			"%" PRIu64 " %010.3lf "
			// latitude, longitude
			"%.8lf %.8lf "
			// GPS height
			"%.3lf %.3lf "
			// speed and track
			"%.2lf %.2lf "
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
			// speed and track
			speed,track,
			// fix quality, n satellites, HDOP
			fixQuality,nSatellites,hdop
			);
	strcpy(s,str);
	return n;
}


void GPSdata::sprint_csv(char *s) {
	 char str[1024],str1[256];

	 TimeStampedData::sprint_csv(str1);
	 sprintf(str,"GPS,%s"
			 // date, time
			 "%" PRIu64 ",%010.3lf,"
			 // latitude, longitude
			 "%.8lf,%.8lf,"
			 // GPS height
			 "%.3lf,%.3lf,"
			 // speed and track
			 "%.2lf,%.2lf,"
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
			 // speed and track
			 speed,track,
			 // fix quality, n satellites, HDOP
			 fixQuality,nSatellites,hdop
			 );
	 strcpy(s,str);
}

void GPSdata::sread(char *s) {
	char str[64];
	char name[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			// date, time
			" %" SCNu64 " %lf"
			// latitude, longitude
			" %lf %lf"
			// GPS height
			" %lf %lf"
			// speed and track
			" %lf %lf"
			// fix quality, n satellites, HDOP
			" %" SCNu8 " %" SCNu8 " %lf"
			"",
			// packet code
			str,
			// time stamp
			name,&mTimeStamp,
			// date and time
			&date,&time,
			// latitude, longitude
			&latitude,&longitude,
			// GPS height
			&heightAboveEllipsoid,&heightAboveMSL,
			// speed and track
			&speed,&track,
			// fix quality, n satellites, HDOP
			&fixQuality,&nSatellites,&hdop
			);
	mName=name;

}

int GPSdata::sprint_NMEA_GGA(char *s) {
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

	sprintf(packet,"$GPGGA,%09.2lf,%011.6lf,%s,%012.6lf,%s,1,07,1.0,0.0,M,0.0,M,,*",
			time,fabs(latDegMin),strNS.c_str(),fabs(lonDegMin),strEW.c_str());

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

	int n=sprintf(s,"$GPGGA,%09.2lf,%011.6lf,%s,%012.6lf,%s,1,07,1.0,0.0,M,0.0,M,,*%x\r\n",
				time,fabs(latDegMin),strNS.c_str(),fabs(lonDegMin),strEW.c_str(),NMEAchecksum);
	return n;

}

int GPSdata::sprint_NMEA_VTG(char *s) {
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
	sprintf(packet,"$GPVTG,%.2lf,T,,M,%.2lf,N,%.2lf,K,D*",
			mod360(track),speedKnots,speedKmh);

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

	int n=sprintf(s,"$GPVTG,%.2lf,T,,M,%.2lf,N,%.2lf,K,D*%x\r\n",
			mod360(track),speedKnots,speedKmh,NMEAchecksum);
	return n;
}


int GPSdata::sprint_NMEA_ZDA(char *s) {
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
	sprintf(packet,"$GPZDA,%010.3lf,%02d,%02d,%d,01,00*",
			time,(int)dd,(int)mm,(int)yyyy);

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

	int n=sprintf(s,"$GPZDA,%010.3lf,%02d,%02d,%d,01,00*%x\r\n",
			time,(int)dd,(int)mm,(int)yyyy,NMEAchecksum);
	return n;
}


double GPSdata::deg_dec_2_deg_min(double degDec) {
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
