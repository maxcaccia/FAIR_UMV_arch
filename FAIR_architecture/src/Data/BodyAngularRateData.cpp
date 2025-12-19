/*
 * BodyAngularRateData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "BodyAngularRateData.h"

BodyAngularRateData::BodyAngularRateData(string name):TimeStampedData(name) {
	mPacketCode=BODY_ANGULAR_RATE_CODE;
	reset();
}

BodyAngularRateData::BodyAngularRateData(void):TimeStampedData() {
	// TODO Auto-generated constructor stub
	mPacketCode=BODY_ANGULAR_RATE_CODE;
	reset();
}

void BodyAngularRateData::reset(void) {
	p=0.;
	q=0.;
	r=0.;
	pStd=0;
	qStd=0;
	rStd=0;
}

BodyAngularRateData & BodyAngularRateData::operator = (const BodyAngularRateData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// angular velocity
	p=obj.p;
	q=obj.q;
	r=obj.r;
	// angular velocity stdev
	pStd=obj.pStd;
	qStd=obj.qStd;
	rStd=obj.rStd;
	// return a reference to myself
	return *this;
}


void BodyAngularRateData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_roll_rate,"					// CF
			"%s_pitch_rate,"					// CF
			"%s_yaw_rate,"					// CF
			"%s_roll_rate_std,"				// CF
			"%s_pitch_rate_std,"			// CF
			"%s_yaw_rate_std,",				// CF
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

void BodyAngularRateData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_roll_rate_starboard_down,"								// CF
			"platform_pitch_rate_fore_up,"								// CF
			"platform_yaw_rate_fore_starboard,"									// CF
			"platform_roll_rate_starboard_down_stdev,"							// CF
			"platform_pitch_rate_fore_up_stdev,"							// CF
			"platform_yaw_rate_fore_starboard_stdev,"							// CF
			,strTS);
	strcpy(s,str);
}

int BodyAngularRateData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// angular velocity
			"%.2lf %.2lf %.2lf "
			// angular velocity stdev
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamped data
			str1,
			// angular velocity stdev
			p,q,r,
			// angular velocity stdev
			pStd,qStd,rStd
			);
	strcpy(s,str);
	return n;
}

void BodyAngularRateData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// angular velocity
			"%.2lf,%.2lf,%.2lf,"
			// angular velocity stdev
			"%.2lf,%.2lf,%.2lf,",
			// time stamped data
			str1,
			// angular velocity
			p,q,r,
			// angular velocity stdev
			pStd,qStd,rStd
			);
	strcpy(s,str);
}


void BodyAngularRateData::sread(char *s) {
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
			// angular velocity
			" %lf %lf %lf"
			// angular velocity stdev
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
			// angular velocity
			&p,&q,&r,
			// angular velocity stdev
			&pStd,&qStd,&rStd
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void BodyAngularRateData::assign(AhrsData ahrsData)  {
	// angular velocity
	p=ahrsData.p;
	q=ahrsData.q;
	r=ahrsData.r;
}




