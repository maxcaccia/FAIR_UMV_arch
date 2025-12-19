/*
 * BodyLinearRateData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "BodyLinearRateData.h"

BodyLinearRateData::BodyLinearRateData(string name):TimeStampedData(name) {
	mPacketCode=BODY_LINEAR_RATE_CODE;
	reset();
}

BodyLinearRateData::BodyLinearRateData(void):TimeStampedData() {
	// TODO Auto-generated constructor stub
	mPacketCode=BODY_LINEAR_RATE_CODE;
	reset();
}

void BodyLinearRateData::reset(void) {
	u=0.;
	v=0.;
	w=0.;;
	uStd=0;
	vStd=0;
	wStd=0;
}

BodyLinearRateData & BodyLinearRateData::operator = (const BodyLinearRateData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// velocity
	u=obj.u;
	v=obj.v;
	w=obj.w;
	// velocity stdev
	uStd=obj.uStd;
	vStd=obj.vStd;
	wStd=obj.wStd;
	// return a reference to myself
	return *this;
}


void BodyLinearRateData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_surge_rate_fore,"
			"%s_sway_rate_starboard,"
			"%s_heave_rate_down,"
			"%s_surge_rate_fore_std,"					// CF
			"%s_sway_rate_starboard_std,"					// CF
			"%s_heave_rate_down_std,",					// CF
			strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void BodyLinearRateData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_surge_rate_fore,"
			"platform_sway_rate_starboard,"
            "platform_heave_rate_down,"
			"platform_surge_rate_fore_stdev,"								// CF
			"platform_sway_rate_starboard_stdev,"								// CF
			"platform_heave_rate_down_stdev,"									// CF
			,strTS);
	strcpy(s,str);
}

int BodyLinearRateData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// velocity
			"%.2lf %.2lf %.2lf "
			// velocity stdev
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamped data
			str1,
			// velocity
			u,v,w,
			// velocity stdev
			uStd,vStd,wStd
			);
	strcpy(s,str);
	return n;
}

void BodyLinearRateData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// velocity
			"%.2lf,%.2lf,%.2lf,"
			// velocity stdev
			"%.2lf,%.2lf,%.2lf,",
			// time stamped data
			str1,
			// velocity
			u,v,w,
			// velocity stdev
			uStd,vStd,wStd
			);
	strcpy(s,str);
}


void BodyLinearRateData::sread(char *s) {
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
			// velocity stdev
			" %lf %lf %lf"
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
			// velocity stdev
			&uStd,&vStd,&wStd
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void BodyLinearRateData::assign(DvlData dvlData)  {
	// velocity
	u=dvlData.u;
	v=dvlData.v;
	w=dvlData.w;
}

void BodyLinearRateData::assign(AhrsData ahrsData)  {
	// velocity
	u=ahrsData.u;
	v=ahrsData.v;
	w=ahrsData.w;
}



