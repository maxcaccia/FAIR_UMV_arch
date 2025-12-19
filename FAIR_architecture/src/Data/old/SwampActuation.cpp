/*
 * SwampActuation.cpp
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
 
#include "SwampActuation.h"
#include <stdio.h>
 
SwampActuation::SwampActuation(string name):TimeStampedData(name) {
	reset();
}

void SwampActuation::reset(void) {
	alpha=0;
	n=0;
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
	sprintf(str,"packet_code,%s"
			"SWAMP_reference_azimuth,"
			"SWAMP_reference_rpm,"
			"SWAMP_reference_azimuth_RL,"
			"SWAMP_reference_azimuth_FL,"
			"SWAMP_reference_azimuth_RR,"
			"SWAMP_reference_azimuth_FR,"
			"SWAMP_reference_rpm_RL,"
			"SWAMP_reference_rpm_FL,"
			"SWAMP_reference_rpm_RR,"
			"SWAMP_reference_rpm_FR,"
			,strTS);
	strcpy(s,str);
}


void SwampActuation::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
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
	number=sprintf(str,"SWAMP_ACTUATION %s"
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
	sprintf(str,"SWAMP_ACTUATION,%s"
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
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			// Manual mode references
			"%lf %lf "
			// Minion references: azimuth
			"%lf %lf %lf %lf "
			// Minion references rpm
			"%lf %lf %lf %lf "
			"",
			str,
			// time stamp
			name,&mTimeStamp,
			// Manual mode references
			&alpha,&n,
			// Minion references: azimuth
			&alphaRL,&alphaFL,&alphaRR,&alphaFR,
			// Minion references rpm
			&nRL,&nFL,&nRR,&nFR
			);
	mName=name;
//	strcpy(s,str);
}

