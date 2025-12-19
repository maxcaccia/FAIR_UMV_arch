/*
 * P2LSMstate.cpp
 *
 *  Created on: 10 February 2025
 *      Author: massimo
 */
 
#include "P2LSMstate.h"
#include <stdio.h>

P2LSMstate::P2LSMstate(void):TimeStampedData() {
	mPacketCode=P2LSM_STATE_CODE;
	reset();
}

P2LSMstate::P2LSMstate(string name):TimeStampedData(name) {
	mPacketCode=P2LSM_STATE_CODE;
	reset();
}

void P2LSMstate::reset(void) {
	mActiveLogicalPosition="null";
	mActiveLogicalAttitude="null";
	mActiveLogicalSpeed="null";
	mActiveLogicalLinearRate="null";
	mActiveLogicalAngularRate="null";
}

P2LSMstate & P2LSMstate::operator = (const P2LSMstate &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);

	mActiveLogicalPosition=obj.mActiveLogicalPosition;
	mActiveLogicalAttitude=obj.mActiveLogicalAttitude;
	mActiveLogicalSpeed=obj.mActiveLogicalSpeed;
	mActiveLogicalLinearRate=obj.mActiveLogicalLinearRate;
	mActiveLogicalAngularRate=obj.mActiveLogicalAngularRate;

	// return a reference to myself
	return *this;
}

void P2LSMstate::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_position,"
			"%s_attitude,"
			"%s_speed,"
			"%s_linear_rate,"
			"%s_angular_rate,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void P2LSMstate::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"P2LSM_position,"
			"P2LSM_attitude,"
			"P2LSM_speed,"
			"P2LSM_linear_rate,"
			"P2LSM_angular_rate,"
			,strTS
			);
	strcpy(s,str);
}


int P2LSMstate::sprint(char *s) {
	char str[1024],str1[256];
	int number;

	TimeStampedData::sprint(str1);
	number=sprintf(str,"%s"
			// enable mode function config
			"%s %s %s %s %s "
			" ",
			// time stamp
			str1,
			// active sensors
			mActiveLogicalPosition.c_str(),mActiveLogicalAttitude.c_str(),mActiveLogicalSpeed.c_str(),
			mActiveLogicalLinearRate.c_str(),mActiveLogicalAngularRate.c_str()
			);
	strcpy(s,str);
	return number;
}


void P2LSMstate::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// enable mode function config
			"%s,%s,%s,%s,%s,",
			// time stamp
			str1,
			// active sensors
			mActiveLogicalPosition.c_str(),mActiveLogicalAttitude.c_str(),mActiveLogicalSpeed.c_str(),
			mActiveLogicalLinearRate.c_str(),mActiveLogicalAngularRate.c_str()
			);
	strcpy(s,str);
}

void P2LSMstate::sread(char *s) {
	char str[64];
	char name[64];
	uint8_t valid;
	char activeLogicalPosition[64],activeLogicalAttitude[64],activeLogicalSpeed[64];
	char activeLogicalLinearRate[64],activeLogicalAngularRate[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %lf "			// name timeStamp
			// valid flag
			" %" SCNu8
			// active sensors
			"%s %s %s %s %s "
			"",
			str,
			// time stamp
			name,&mTimeStamp,&valid,
			// enable mode function config
			activeLogicalPosition,activeLogicalAttitude,activeLogicalSpeed,
			activeLogicalLinearRate,activeLogicalAngularRate
			);
	mName=name;
	mValid = (bool)valid;
	mActiveLogicalPosition = activeLogicalPosition;
	mActiveLogicalAttitude=activeLogicalAttitude;
	mActiveLogicalSpeed=activeLogicalSpeed;
	mActiveLogicalLinearRate=activeLogicalLinearRate;
	mActiveLogicalAngularRate=activeLogicalAngularRate;
//	strcpy(s,str);
}

