/*
 * DvlData.cpp
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
 
#include <stdio.h>

#include "DvlData.h"
 
DvlData::DvlData(string name):TimeStampedData(name) {
	reset();
	mPacketCode=DVL_CODE;
}

DvlData::DvlData(void):TimeStampedData() {
	reset();
	mPacketCode=DVL_CODE;
}


void DvlData::reset(void) {
	u=0;
	v=0;
	w=0;
	rho0=0.;
	rho1=0.;
	rho2=0.;
	rho3=0.;
}

DvlData & DvlData::operator = (const DvlData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// velocity
	u=obj.u;
	v=obj.v;
	w=obj.w;
	// range from bottom
	rho0=obj.rho0;
	rho1=obj.rho1;
	rho2=obj.rho2;
	rho3=obj.rho3;
	// return a reference to myself
	return *this;
}

void DvlData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_surge_rate_fore,"
			"%s_sway_rate_starboard,"
			"%s_heave_rate_down,"
			"%s_echo_sounder_range_0,"
			"%s_echo_sounder_range_1,"
			"%s_echo_sounder_range_2,"
			"%s_echo_sounder_range_3,"
			,strTS
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


void DvlData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_surge_rate_fore,"
			"platform_sway_rate_starboard,"
            "platform_heave_rate_down,"
			"echo_sounder_range"
			"echo_sounder_range,"
            "echo_sounder_range,"
			"echo_sounder_range,"
            ,strTS
			);
	strcpy(s,str);
}


int DvlData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// velocity
			"%.2lf %.2lf %.2lf "
			// range from bottom
			"%.2lf %.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// velocity
			u,v,w,
			// range from bottom
			rho0,rho1,rho2,rho3
			);
	strcpy(s,str);
	return n;
}


void DvlData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// velocity
			"%.2lf,%.2lf,%.2lf,"
			// range from bottom
			"%.2lf,%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// velocity
			u,v,w,
			// range from bottom
			rho0,rho1,rho2,rho3
			);
	strcpy(s,str);
}

void DvlData::sread(char *s) {
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
			// velocity
			"%lf %lf %lf "
			// range from bottom
			"%lf %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// velocity
			&u,&v,&w,
			// range from bottom
			&rho0,&rho1,&rho2,&rho3
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}


