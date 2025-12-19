/*
 * AngularPosition.cpp
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
 
#include "AngularPosition.h"
#include <stdio.h>
 
AngularPosition::AngularPosition(string name):TimeStampedData(name) {
	mPacketCode=ANGULAR_POSITION_CODE;
	reset();
}

AngularPosition::AngularPosition(void):TimeStampedData() {
	reset();
	mPacketCode=ANGULAR_POSITION_CODE;
}


void AngularPosition::reset(void) {
	phi=0;
	theta=0;
	psi=0;
}

AngularPosition & AngularPosition::operator = (const AngularPosition &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// angles
	phi=obj.phi;
	theta=obj.theta;
	psi=obj.psi;
	// return a reference to myself
	return *this;
}

void AngularPosition::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_roll,"
			"%s_pitch,"
			"%s_yaw,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void AngularPosition::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_roll_starboard_down,"			//CF
			"platform_pitch_fore_up,"				//CF
			"platform_orientation,"					//CF
 			,strTS
			);
	strcpy(s,str);
}


int AngularPosition::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// angles
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// angles
			phi,theta,psi
			);
	strcpy(s,str);
	return n;
}


void AngularPosition::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// angles
			"%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// angles
			phi,theta,psi
			);
	strcpy(s,str);
}

void AngularPosition::sread(char *s) {

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
			" %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// angles
			&phi,&theta,&psi
			);
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}


bool AngularPosition::fread(FILE *f) {
	char str[64];

    // angles: roll, pitch, yaw
	fscanf(f,"%s %lf",str,&phi);
    if(strcmp(str,"phi")!=0)
    {
        printf("AngularPosition::fread phi expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&theta);
    if(strcmp(str,"theta")!=0)
    {
        printf("AngularPosition::fread theta expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&psi);
    if(strcmp(str,"psi")!=0)
    {
        printf("AngularPosition::fread psi expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	return true;
}

