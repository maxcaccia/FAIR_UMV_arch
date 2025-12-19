/*
 * SwampActuation.cpp
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
 
#include "SwampActuation.h"
#include <stdio.h>

SwampActuation::SwampActuation(void):TimeStampedData() {
	mPacketCode=SWAMP_ACTUATION_CODE;
	reset();
}

SwampActuation::SwampActuation(string name):TimeStampedData(name) {
	mPacketCode=SWAMP_ACTUATION_CODE;
	reset();
}

void SwampActuation::reset(void) {
	alpha=0;
	n=0;
//	for(int i=0;i<SWAMP_N;i++) {
//		alphaMinion[i]=0.;
//		nMinion[i]=0;
//		azimuthPowerMinion[i]=0;
//		pumpPowerMinion[i]=0;
//
//	}
	alphaRL=0;
	nRL=0;
	alphaFL=0;
	nFL=0;
	alphaRR=0;
	nRR=0;
	alphaFR=0;
	nFR=0;
}

SwampActuation & SwampActuation::operator = (const SwampActuation &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// Manual mode references
	alpha=obj.alpha;
	n=obj.n;
	// Minion references
	alphaRL=obj.alphaRL;
	nRL=obj.nRL;
	alphaFL=obj.alphaFL;
	nFL=obj.nFL;
	alphaRR=obj.alphaRR;
	nRR=obj.nRR;
	alphaFR=obj.alphaFR;
	nFR=obj.nFR;
	// return a reference to myself
	return *this;
}

void SwampActuation::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_azimuth,"
			"%s_rpm,"
			"%s_azimuth_RL,"
			"%s_azimuth_FL,"
			"%s_azimuth_RR,"
			"%s_azimuth_FR,"
			"%s_rpm_RL,"
			"%s_rpm_FL,"
			"%s_rpm_RR,"
			"%s_rpm_FR,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
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


void SwampActuation::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"motor_reference_azimuth,"
			"motor_reference_speed,"
			"motor_reference_azimuth,"
			"motor_reference_azimuth,"
			"motor_reference_azimuth,"
			"motor_reference_azimuth,"
			"motor_reference_speed,"
			"motor_reference_speed,"
			"motor_reference_speed,"
			"motor_reference_speed,"
			,strTS
			);
	strcpy(s,str);
}


int SwampActuation::sprint(char *s) {
	char str[1024],str1[256];
	int number;

	TimeStampedData::sprint(str1);
	number=sprintf(str,"%s"
			// Manual mode references
			"%.2lf %.2lf "
			// Minion references: azimuth
			"%.2lf %.2lf %.2lf %.2lf "
			// Minion references rpm
			"%.2lf %.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// Manual mode references
			alpha,n,
			// Minion references: azimuth
			alphaRL,alphaFL,alphaRR,alphaFR,
			// Minion references rpm
			nRL,nFL,nRR,nFR
			);
	strcpy(s,str);
	return number;
}


void SwampActuation::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// Manual mode references
			"%.2lf,%.2lf,"
			// Minion references: azimuth
			"%.2lf,%.2lf,%.2lf,%.2lf,"
			// Minion references rpm
			"%.2lf,%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// Manual mode references
			alpha,n,
			// Minion references: azimuth
			alphaRL,alphaFL,alphaRR,alphaFR,
			// Minion references rpm
			nRL,nFL,nRR,nFR
			);
	strcpy(s,str);
}

void SwampActuation::sread(char *s) {
	char str[64];
	char name[64];
	uint8_t valid;
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %lf "			// name timeStamp
			// valid flag
			" %" SCNu8
			// Manual mode references
			"%lf %lf "
			// Minion references: azimuth
			"%lf %lf %lf %lf "
			// Minion references rpm
			"%lf %lf %lf %lf "
			"",
			str,
			// time stamp
			name,&mTimeStamp,&valid,
			// Manual mode references
			&alpha,&n,
			// Minion references: azimuth
			&alphaRL,&alphaFL,&alphaRR,&alphaFR,
			// Minion references rpm
			&nRL,&nFL,&nRR,&nFR
			);
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}

