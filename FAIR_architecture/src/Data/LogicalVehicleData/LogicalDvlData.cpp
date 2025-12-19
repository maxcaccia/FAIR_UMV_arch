/*
 * LogicalDvlData.cpp
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
 
#include "LogicalDvlData.h"

LogicalDvlData::LogicalDvlData(string name):DvlData(name) {
	mPacketCode=LOGICAL_DVL_CODE;
	reset();
}

LogicalDvlData::LogicalDvlData(void):DvlData() {
	// TODO Auto-generated constructor stub
	mPacketCode=LOGICAL_DVL_CODE;
	reset();
}

void LogicalDvlData::reset(void) {
	DvlData::reset();
	uStd=0; // surge [m/s]
	vStd=0; // sway [m/s]
	wStd=0; // heave [m/s]
	rhoStd=0; // range [m]
}

LogicalDvlData & LogicalDvlData::operator = (const LogicalDvlData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	DvlData::operator =(obj);
	// velocity
	uStd=obj.uStd;
	vStd=obj.vStd;
	wStd=obj.wStd;
	// range from bottom
	rhoStd=obj.rhoStd;
	// return a reference to myself
	return *this;
}


void LogicalDvlData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	DvlData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_surge_rate_fore_std,"					// CF
			"%s_sway_rate_starboard_std,"					// CF
			"%s_heave_rate_down_std,"					// CF
			"%s_echo_sounder_range_std,",				// CF
			strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void LogicalDvlData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	DvlData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_surge_rate_fore_stdev,"								// CF
			"platform_sway_rate_starboard_stdev,"								// CF
			"platform_heave_rate_down_stdev,"									// CF
			"echo_sounder_range_stdev,"							// CF
			,strTS);
	strcpy(s,str);
}

int LogicalDvlData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	DvlData::sprint(str1);
	n=sprintf(str,"%s"
			// velocity stdev
			"%.2lf %.2lf %.2lf "
			// range from bottom stdev
			"%.2lf "
			" ",
			// dvl data
			str1,
			// velocity stdev
			uStd,vStd,wStd,
			// range from bottom stdev
			rhoStd
			);
	strcpy(s,str);
	return n;
}

void LogicalDvlData::sprint_csv(char *s) {
	char str[2048],str1[256];

	DvlData::sprint_csv(str1);
	sprintf(str,"%s"
			// velocity stdev
			"%.2lf,%.2lf,%.2lf,"
			// range from bottom stdev
			"%.2lf,",
			// dvl data
			str1,
			// velocity stdev
			uStd,vStd,wStd,
			// range from bottom stdev
			rhoStd
			);
	strcpy(s,str);
}


void LogicalDvlData::sread(char *s) {
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
			"%lf %lf %lf %lf"
			// velocity stdev
			" %lf %lf %lf"
			// range from bottom stdev
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
			// velocity
			&u,&v,&w,
			// range from bottom
			&rho0,&rho1,&rho2,&rho3,
			// velocity stdev
			&uStd,&vStd,&wStd,
			// angular velocity stdev
			&rhoStd
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalDvlData::assign(DvlData dvlData)  {
	// velocity
	u=dvlData.u;
	v=dvlData.v;
	w=dvlData.w;
	// range from bottom
	rho0=dvlData.rho0;
	rho1=dvlData.rho1;
	rho2=dvlData.rho2;
	rho3=dvlData.rho3;
}




