/*
 * BatteryData.cpp
 *
 *  Created on: May 10, 2023
 *      Author: mc
 */

#include "BatteryData.h"

BatteryData::BatteryData(string name):TimeStampedData(name) {
	// TODO Auto-generated constructor stub
	mPacketCode=BATTERY_CODE;
	reset();
}

BatteryData::BatteryData(void):TimeStampedData() {
	// TODO Auto-generated constructor stub
	mPacketCode=BATTERY_CODE;
	reset();
}

BatteryData::~BatteryData() {
	// TODO Auto-generated destructor stub
}

void BatteryData::reset(void) {
	voltage=0;
}

BatteryData & BatteryData::operator = (const BatteryData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	voltage=obj.voltage;
	// return a reference to myself
	return *this;
}

void BatteryData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_battery_voltage,"
			,strTS
			,mName.c_str()
			);
	strcpy(s,str);
}

void BatteryData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"voltage,"
			,strTS);
	strcpy(s,str);
}

int BatteryData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// voltage
			"%.2lf"
			" ",
			// time stamp
			str1,
			// voltage
			voltage);
	strcpy(s,str);
	return n;
}

void BatteryData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// voltage
			"%.2lf"
			",",
			// time stamp
			str1,
			// voltage
			voltage);
	strcpy(s,str);
}


void BatteryData::sread(char *s) {
	char packetCode[64];
	char name[64];
	uint8_t valid;
	sscanf(s,// packet_code
			"%s "
			// name
			"%s "
			// time stamp
			"%lf"
			// valid flag
			" %" SCNu8
			// voltage
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
			// voltage
			&voltage
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

