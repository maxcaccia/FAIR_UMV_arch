/*
 * NormalisedRpm.cpp
 *
 *  Created on: 26 March 2025
 *      Author: massimo
 */
 
#include "NormalisedRpm.h"
#include <stdio.h>
 
NormalisedRpm::NormalisedRpm(string name):TimeStampedData(name) {
	mPacketCode=NORMALISED_RPM_CODE;
	reset();
}

NormalisedRpm::NormalisedRpm(void):TimeStampedData() {
	mPacketCode=NORMALISED_RPM_CODE;
	reset();
}


void NormalisedRpm::reset(void) {
	mLeftRpm=0;
	mRightRpm=0;
}

NormalisedRpm & NormalisedRpm::operator = (const NormalisedRpm &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// applied force to the vehicle rigid body
	mLeftRpm=obj.mLeftRpm;
	// applied moment to the vehicle rigid body
	mRightRpm=obj.mRightRpm;
	// return a reference to myself
	return *this;
}

void NormalisedRpm::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_left_normalised_rpm,"
			"%s_right_normalised_rpm,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void NormalisedRpm::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"normalised_rpm,"			//No CF	(RF)
			"normalised_rpm,"					//No CF	(RF)
			,strTS
			);
	strcpy(s,str);
}


int NormalisedRpm::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// left rpm
			"%.2lf "
			// right rpm
			"%.2lf"
			" ",
			// time stamp
			str1,
			// keft rpm
			mLeftRpm,
			// right rpm
			mRightRpm // [N m]
			);
	strcpy(s,str);
	return n;
}


void NormalisedRpm::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// left rpm
			"%.2lf,"
			// right rpm
			"%.2lf"
			",",
			// time stamp
			str1,
			// left rpm
			mLeftRpm,
			// right rpm
			mRightRpm // [N m]
			);
	strcpy(s,str);
}

void NormalisedRpm::sread(char *s) {
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
			// applied force to the vehicle rigid body
			" %lf"
			// applied moment to the vehicle rigid body
			" %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// applied force to the vehicle rigid body
			&mLeftRpm,
			// applied moment to the vehicle rigid body
			&mRightRpm // [N m]
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}

