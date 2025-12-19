/*
 * BodyAttitudeData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "BodyAttitudeData.h"

BodyAttitudeData::BodyAttitudeData(string name):TimeStampedData(name) {
	mPacketCode=BODY_ATTITUDE_CODE;
	reset();
}

BodyAttitudeData::BodyAttitudeData(void):TimeStampedData() {
	// TODO Auto-generated constructor stub
	mPacketCode=BODY_ATTITUDE_CODE;
	reset();
}

void BodyAttitudeData::reset(void) {
	phi=0.;
	theta=0.;
	psi=0.;
	phiStd=0;
	thetaStd=0;
	psiStd=0;
}

BodyAttitudeData & BodyAttitudeData::operator = (const BodyAttitudeData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// angle
	phi=obj.phi;
	theta=obj.theta;
	psi=obj.psi;
	// angle stdev
	phiStd=obj.phiStd;
	thetaStd=obj.thetaStd;
	psiStd=obj.psiStd;
	// return a reference to myself
	return *this;
}


void BodyAttitudeData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_roll,"					// CF
			"%s_pitch,"					// CF
			"%s_yaw,"					// CF
			"%s_roll_std,"					// CF
			"%s_pitch_std,"					// CF
			"%s_yaw_std,",					// CF
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

void BodyAttitudeData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_roll_starboard_down,"								// CF
			"platform_pitch_fore_up,"								// CF
			"platform_yaw_fore_starboard,"									// CF
			"platform_roll_starboard_down_stdev,"								// CF
			"platform_pitch_fore_up_stdev,"								// CF
			"platform_yaw_fore_starboard_stdev,"									// CF
			,strTS);
	strcpy(s,str);
}

int BodyAttitudeData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// angles
			"%.2lf %.2lf %.2lf "
			// angles stdev
			"%.2lf %.2lf %.2lf "
			" ",
			// time stampoed data
			str1,
			// angles
			phi,theta,psi,
			// angles stdev
			phiStd,thetaStd,psiStd
			);
	strcpy(s,str);
	return n;
}

void BodyAttitudeData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// angles
			"%.2lf,%.2lf,%.2lf,"
			// angles stdev
			"%.2lf,%.2lf,%.2lf,",
			// time stamped data
			str1,
			// angles
			phi,theta,psi,
			// angles stdev
			phiStd,thetaStd,psiStd
			);
	strcpy(s,str);
}


void BodyAttitudeData::sread(char *s) {
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
			// angles
			" %lf %lf %lf"
			// angles stdev
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
			// angles
			&phi,&theta,&psi,
			// angles stdev
			&phiStd,&thetaStd,&psiStd
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void BodyAttitudeData::assign(AhrsData ahrsData)  {
	// angles
	phi=ahrsData.phi;
	theta=ahrsData.theta;
	psi=ahrsData.psi;
}




