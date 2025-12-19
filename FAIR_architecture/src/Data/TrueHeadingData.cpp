/*
 * TrueHeadingData.cpp
 *
 *  Created on: 24 October 2023
 *      Author: massimo
 */
 
#include "TrueHeadingData.h"
#include <stdio.h>
 
TrueHeadingData::TrueHeadingData(string name):TimeStampedData(name)  {
	mPacketCode=TRUE_HEADING_CODE;
	reset();
}

TrueHeadingData::TrueHeadingData(void):TimeStampedData()  {
	mPacketCode=TRUE_HEADING_CODE;
	reset();
}

void TrueHeadingData::reset(void) {
	trueHeading=0.;
}

TrueHeadingData & TrueHeadingData::operator = (const TrueHeadingData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// true heading
	trueHeading=obj.trueHeading;
	// return a reference to myself
	return *this;
}


void TrueHeadingData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_true_heading,"					// No CF
			,strTS
			,mName.c_str()
			);
	strcpy(s,str);
}

void TrueHeadingData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"true_heading,"									// No CF
			,strTS
			);
	strcpy(s,str);
}

int TrueHeadingData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// true heading
			"%.2lf "
			" ",
			// time stamp
			str1,
			// true heading
			trueHeading
			);
	strcpy(s,str);
	return n;
}


void TrueHeadingData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			 // true heading
			 "%.2lf,",
			 // time stamp
			 str1,
			 // true heading
			 trueHeading
			 );
	 strcpy(s,str);
}

void TrueHeadingData::sread(char *s) {
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
			// true heading
			" %lf"
			"",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// true heading
			&trueHeading
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

int TrueHeadingData::sprint_NMEA_HDT(char *s) {
	char packet[256];
	// sprint trueHeading [deg] - True
	// sprint "T"
	sprintf(packet,"$GPHDT,%.3lf,T*",
			mod360(trueHeading));

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

	int n=sprintf(s,"$GPHDT,%.3lf,T*%x\r\n",
			mod360(trueHeading),NMEAchecksum);
	return n;

}

