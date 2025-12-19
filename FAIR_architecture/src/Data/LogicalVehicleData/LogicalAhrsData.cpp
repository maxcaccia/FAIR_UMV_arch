/*
 * LogicalAhrsData.cpp
 *
 *  Created on: 9 January 2025
 *      Author: massimo
 */
 
#include "LogicalAhrsData.h"

LogicalAhrsData::LogicalAhrsData(string name):AhrsData(name) {
	mPacketCode=LOGICAL_AHRS_CODE;
	reset();
}

LogicalAhrsData::LogicalAhrsData(void):AhrsData() {
	// TODO Auto-generated constructor stub
	mPacketCode=LOGICAL_AHRS_CODE;
	reset();
}

void LogicalAhrsData::reset(void) {
	AhrsData::reset();
	phiStd=0; // roll [deg]
	thetaStd=0; // pitch [deg]
	psiStd=0; // yaw [deg]
	pStd=0; // roll rate [deg/s]
	qStd=0; // pitch rate [deg/s]
	rStd=0; // yaw rate [deg/s]
}

LogicalAhrsData & LogicalAhrsData::operator = (const LogicalAhrsData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	AhrsData::operator =(obj);
	// angles
	phiStd=obj.phiStd;
	thetaStd=obj.thetaStd;
	psiStd=obj.psiStd;
	// angular velocity
	pStd=obj.pStd;
	qStd=obj.qStd;
	rStd=obj.rStd;
	// return a reference to myself
	return *this;
}


void LogicalAhrsData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	AhrsData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_roll_std,"					// CF
			"%s_pitch_std,"					// CF
			"%s_yaw_std,"					// CF
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

void LogicalAhrsData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	AhrsData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_roll_starboard_down_stdev,"								// CF
			"platform_pitch_fore_up_stdev,"								// CF
			"platform_yaw_fore_starboard_stdev,"									// CF
			"platform_roll_rate_starboard_down_stdev,"							// CF
			"platform_pitch_rate_fore_up_stdev,"							// CF
			"platform_yaw_rate_fore_starboard_stdev,"							// CF
			,strTS);
	strcpy(s,str);
}

int LogicalAhrsData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	AhrsData::sprint(str1);
	n=sprintf(str,"%s"
			// angles stdev
			"%.2lf %.2lf %.2lf "
			// angular velocity stdev
			"%.2lf %.2lf %.2lf "
			" ",
			// ahrs data
			str1,
			// angles stdev
			phiStd,thetaStd,psiStd,
			// angular velocity stdev
			pStd,qStd,rStd
			);
	strcpy(s,str);
	return n;
}

void LogicalAhrsData::sprint_csv(char *s) {
	char str[2048],str1[256];

	AhrsData::sprint_csv(str1);
	sprintf(str,"%s"
			// angles stdev
			"%.2lf,%.2lf,%.2lf,"
			// angular velocity stdev
			"%.2lf,%.2lf,%.2lf,",
			// ahrs data
			str1,
			// angles stdev
			phiStd,thetaStd,psiStd,
			// angular velocity stdev
			pStd,qStd,rStd
			);
	strcpy(s,str);
}


void LogicalAhrsData::sread(char *s) {
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
			// linear accelerations
			" %lf %lf %lf"
			// angular velocity
			" %lf %lf %lf"
			// magnetic components
			" %lf %lf %lf"
			// linear velocity
			" %lf %lf %lf"
			// angles stdev
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
			// angles
			&phi,&theta,&psi,
			// linear accelerations
			&uDot,&vDot,&wDot,
			// angular velocity
			&p,&q,&r,
			// magnetic components
			&magnX,&magnY,&magnZ,
			// linear velocity
			&u,&v,&w,
			// angles stdev
			&phiStd,&thetaStd,&psiStd,
			// angular velocity stdev
			&pStd,&qStd,&rStd
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalAhrsData::assign(AhrsData ahrsData)  {
	// angles
	phi=ahrsData.phi;
	theta=ahrsData.theta;
	psi=ahrsData.psi;
	// linear accelerations
	uDot=ahrsData.uDot;
	vDot=ahrsData.vDot;
	wDot=ahrsData.wDot;
	// angular velocity
	p=ahrsData.p;
	q=ahrsData.q;
	r=ahrsData.r;
	// magnetic components
	magnX=ahrsData.magnX;
	magnY=ahrsData.magnY;
	magnZ=ahrsData.magnZ;
	// linear velocity
	u=ahrsData.u;
	v=ahrsData.v;
	w=ahrsData.w;
}




