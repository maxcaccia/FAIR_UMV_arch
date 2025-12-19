/*
 * TCAstate.cpp
 *
 *  Created on:7 August 2024
 *      Author: massimo
 */
 
#include "TCAstate.h"
#include <stdio.h>

TCAstate::TCAstate(void):TimeStampedData() {
	mPacketCode=TCA_STATE_CODE;
	reset();
}

TCAstate::TCAstate(string name):TimeStampedData(name) {
	mPacketCode=TCA_STATE_CODE;
	reset();
}

void TCAstate::reset(void) {
	mPropulsionClass = UNDEFINED;
	mEnable=false;
	mMode="null";
	mFunction="null";
	mConfig="null";
}

TCAstate & TCAstate::operator = (const TCAstate &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);

	mPropulsionClass=obj.mPropulsionClass;
	mEnable=obj.mEnable;
	mMode=obj.mMode;
	mFunction=obj.mFunction;
	mConfig=obj.mConfig;

	// return a reference to myself
	return *this;
}

void TCAstate::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_class,"
			"%s_enable,"
			"%s_mode,"
			"%s_function,"
			"%s_config,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void TCAstate::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"TCA_class,"
			"TCA_enable,"
			"TCA_mode,"
			"TCA_function,"
			"TCA_config,"
			,strTS
			);
	strcpy(s,str);
}


int TCAstate::sprint(char *s) {
	char str[1024],str1[256];
	int number;

	TimeStampedData::sprint(str1);
	number=sprintf(str,"%s"
			// class enable mode function config
			"%d %d %s %s %s "
			" ",
			// time stamp
			str1,
			// enable mode function config
			(int)mPropulsionClass,(int)mEnable,mMode.c_str(),mFunction.c_str(),mConfig.c_str()
			);
	strcpy(s,str);
	return number;
}


void TCAstate::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// enable mode function config
			"%d,%d,%s,%s,%s,",
			// time stamp
			str1,
			// class enable mode function config
			(int)mPropulsionClass,(int)mEnable,mMode.c_str(),mFunction.c_str(),mConfig.c_str()
			);
	strcpy(s,str);
}

void TCAstate::sread(char *s) {
	char str[64];
	char name[64];
	int cl;
	int enable;
	uint8_t valid;
	char mode[64],func[64],conf[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %lf "			// name timeStamp
			// valid flag
			" %" SCNu8
			// class enable mode function config
			"%d %d %s %s %s "
			"",
			str,
			// time stamp
			name,&mTimeStamp,&valid,
			// enable mode function config
			&cl,&enable,mode,func,conf
			);
	mName=name;
	mValid = (bool)valid;
	mPropulsionClass = (PropulsionClass) cl;
	mEnable = (bool)enable;
	mMode=mode;
	mFunction=func;
	mConfig=conf;
//	strcpy(s,str);
}

