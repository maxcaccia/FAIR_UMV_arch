/*
 * SwampTlm.cpp
 *
 *  Created on: 20 December 2023
 *      Author: massimo
 */
 
#include "SwampTlm.h"
#include <stdio.h>
 
SwampTlm::SwampTlm(string name):TimeStampedData(name) {
	reset();
}

SwampTlm::SwampTlm(void):TimeStampedData() {
	reset();
}


void SwampTlm::reset(void) {
	latitude=0;
	longitude=0;
	phi=0;
	theta=0;
	psi=0;
	speed=0;
	groundSpeed=0;
	track=0;
	p=0;
	q=0;
	r=0;
	alphaFL=0;
	nRefFL=0;
	nFL=0;
	currentFL=0;

	alphaFR=0;
	nRefFR=0;
	nFR=0;
	currentFR=0;

	alphaRR=0;
	nRefRR=0;
	nRR=0;
	currentRR=0;

	alphaRL=0;
	nRefRL=0;
	nRL=0;
	currentRL=0;
}

SwampTlm & SwampTlm::operator = (const SwampTlm &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// latitude, longitude
	latitude=obj.latitude;
	longitude=obj.longitude;
	// angles
	phi=obj.phi;
	theta=obj.theta;
	psi=obj.psi;
	// linear velocity and speed (NED)
	speed=obj.speed;
	groundSpeed=obj.groundSpeed;
	track=obj.track;
	// angular velocity
	p=obj.p;
	q=obj.q;
	r=obj.r;
	// Minion FL
	alphaFL=obj.alphaFL; // azimuth [deg]
	nRefFL=obj.nRefFL; // [rpm]
	nFL=obj.nFL; // [rpm]
	currentFL=obj.currentFL;
	// Minion FR
	alphaFR=obj.alphaFR; // azimuth [deg]
	nRefFR=obj.nRefFR; // [rpm]
	nFR=obj.nFR; // [rpm]
	currentFR=obj.currentFR;
	// Minion RR
	alphaRR=obj.alphaRR; // azimuth [deg]
	nRefRR=obj.nRefRR; // [rpm]
	nRR=obj.nRR; // [rpm]
	currentRR=obj.currentRR;
	// Minion FR
	alphaRL=obj.alphaRL; // azimuth [deg]
	nRefRL=obj.nRefRL; // [rpm]
	nRL=obj.nRL; // [rpm]
	currentRL=obj.currentRL;

	// return a reference to myself
	return *this;
}

void SwampTlm::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"SWAMP_latitude,"
			"SWAMP_longitude,"
			"SWAMP_roll,"
			"SWAMP_pitch,"
			"SWAMP_yaw,"
			"SWAMP_speed,"
			"SWAMP_ground_speed,"
			"SWAMP_track,"
			"SWAMP_roll_rate,"
			"SWAMP_pitch_rate,"
			"SWAMP_yaw_rate,"
			"SWAMP_azimuth_FL,"
			"SWAMP_reference_rpm_FL,"
			"SWAMP_rpm_F,L"
			"SWAMP_current_FL,"
			"SWAMP_azimuth_FR,"
			"SWAMP_reference_rpm_FR,"
			"SWAMP_rpm_FR,"
			"SWAMP_current_FR,"
			"SWAMP_azimuth_RR,"
			"SWAMP_reference_rpm_RR,"
			"SWAMP_rpm_RR,"
			"SWAMP_current_RR,"
			"SWAMP_azimuth_RL,"
			"SWAMP_reference_rpm_RL,"
			"SWAMP_rpm_RL,"
			"SWAMP_current_RL,"
			,strTS);
	//sprintf(str,"packet_code,%sSWAMP_latitude,SWAMP_longitude,SWAMP_roll,SWAMP_pitch,SWAMP_yaw,SWAMP_speed,SWAMP_ground_speed,SWAMP_track,SWAMP_roll_rate,SWAMP_pitch_rate,SWAMP_yaw_rate,SWAMP_azimuth_FL,SWAMP_reference_rpm_FL,SWAMP_rpm_FL,SWAMP_current_FL,SWAMP_azimuth_FR,SWAMP_reference_rpm_FR,SWAMP_rpm_FR,SWAMP_current_FR,SWAMP_azimuth_RR,SWAMP_reference_rpm_RR,SWAMP_rpm_RR,SWAMP_current_RR,SWAMP_azimuth_RL,SWAMP_reference_rpm_RL,SWAMP_rpm_RL,SWAMP_current_RL,"
			//,strTS);

	strcpy(s,str);
}


void SwampTlm::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"latitude,"								//CF
			"longitude,"							//CF
			"platform_roll,"						//CF
			"platform_pitch,"						//CF
			"platform_yaw,"							//CF
            "platform_speed,"                       // No CF                       %
            "platform_speed_wrt_ground,"     		// CF
			"platform_track,"						// No CF	(RF)
			"platform_roll_rate,"					// CF
			"platfotm_pitch_rate,"					// CF
			"platform_yaw_rate,"					// CF
			"motor_azimuth,"
			"motor_reference_speed,"
			"motor_speed,"
			"motor_current,"
			"motor_azimuth,"
			"motor_reference_speed,"
			"motor_speed,"
			"motor_current,"
			"motor_azimuth,"
			"motor_reference_speed,"
			"motor_speed,"
			"motor_current,"
			"motor_azimuth,"
			"motor_reference_speed,"
			"motor_speed,"
			"motor_current,"
			,strTS
			);
	strcpy(s,str);
}


int SwampTlm::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"SWAMP_TLM %s"
			// angles
			"%.8lf %.8lf "
			// angles
			"%.2lf %.2lf %.2lf "
			// linear velocity and speed (NED)
			"%.2lf %.2lf %.2lf "
			// angular velocity
			"%.2lf %.2lf %.2lf "
			// Minion FL
			"%.2lf %.2lf %.2lf %.2lf "
			// Minion FR
			"%.2lf %.2lf %.2lf %.2lf "
			// Minion RR
			"%.2lf %.2lf %.2lf %.2lf "
			// Minion RL
			"%.2lf %.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// angles
			phi,theta,psi, // yaw [deg]
			// linear velocity and speed (NED)
			speed,groundSpeed,track,
			// angular speed
			p,q,r,
			// Minion FL
			alphaFL,nRefFL,nFL,currentFL,
			// Minion FR
			alphaFR,nRefFR,nFL,currentFR,
			// Minion RR
			alphaRR,nRefRR,nRR,currentRR,
			// Minion RL
			alphaRL,nRefRL,nRL,currentRL
			);
	strcpy(s,str);
	return n;
}


void SwampTlm::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"SWAMP_TLM,%s"
			// latitude, longitude
			"%.8lf,%.8lf,"
			// angles
			"%.2lf,%.2lf,%.2lf,"
			// linear speed (NED)
			"%.2lf,%.2lf,%.2lf,"
			// angular speed
			"%.2lf,%.2lf,%.2lf,"
			// Minion FL
			"%2lf,%.2lf,%.2lf,%.2lf,"
			// Minion FR
			"%2lf,%.2lf,%.2lf,%.2lf,"
			// Minion RR
			"%2lf,%.2lf,%.2lf,%.2lf,"
			// Minion RL
			"%2lf,%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// angles
			phi,theta,psi, // yaw [deg]
			// linear speed (NED)
			speed,groundSpeed,track,
			// angular speed
			p,q,r,
			// Minion FL
			alphaFL,nRefFL,nFL,currentFL,
			// Minion FR
			alphaFR,nRefFR,nFL,currentFR,
			// Minion RR
			alphaRR,nRefRR,nRR,currentRR,
			// Minion RL
			alphaRL,nRefRL,nRL,currentRL
			);
	strcpy(s,str);
}

void SwampTlm::sread(char *s) {
	char str[64];
	char name[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			// angles
			" %lf %lf"
			// angles
			" %lf %lf %lf"
			// linear velocity and speed (NED)
			" %lf %lf %lf"
			// angular velocity
			" %lf %lf %lf"
			// Minion FL
			" %lf %lf %lf %lf"
			// Minion FR
			" %lf %lf %lf %lf"
			// Minion RR
			" %lf %lf %lf %lf"
			// Minion RL
			" %lf %lf %lf %lf"
			"",
			str,
			// time stamp
			name,&mTimeStamp,
			// latitude, longitude
			&latitude,&longitude,
			// angles
			&phi,&theta,&psi, // yaw [deg]
			// linear velocity and speed (NED)
			&speed,&groundSpeed,&track,
			// angular speed
			&p,&q,&r,
			// Minion FL
			&alphaFL,&nRefFL,&nFL,&currentFL,
			// Minion FR
			&alphaFR,&nRefFR,&nFR,&currentFR,
			// Minion RR
			&alphaRR,&nRefRR,&nRR,&currentRR,
			// Minion RL
			&alphaRL,&nRefRL,&nRL,&currentRL
			);
	mName=name;
//	strcpy(s,str);
}

