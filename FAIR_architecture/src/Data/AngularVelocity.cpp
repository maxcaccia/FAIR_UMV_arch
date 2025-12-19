/*
 * AngularVelocity.cpp
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
 
#include "AngularVelocity.h"
#include <stdio.h>
 
AngularVelocity::AngularVelocity(string name):TimeStampedData(name) {
	mPacketCode=ANGULAR_VELOCITY_CODE;
	reset();
}

AngularVelocity::AngularVelocity(void):TimeStampedData() {
	mPacketCode=ANGULAR_VELOCITY_CODE;
	reset();
}


void AngularVelocity::reset(void) {
	p=0;
	q=0;
	r=0;
}

AngularVelocity & AngularVelocity::operator = (const AngularVelocity &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// angular velocity
	p=obj.p;
	q=obj.q;
	r=obj.r;
	// return a reference to myself
	return *this;
}

void AngularVelocity::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_roll_rate,"
			"%s_pitch_rate,"
			"%s_yaw_rate,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void AngularVelocity::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_roll_rate_starboard_down,"					// CF
			"platfotm_pitch_rate_fore_up,"					// CF
			"platform_yaw_rate_fore_starboard,"					// CF
			,strTS
			);
	strcpy(s,str);
}


int AngularVelocity::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// angular velocity
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// angular speed
			p,q,r
			);
	strcpy(s,str);
	return n;
}


void AngularVelocity::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// angular speed
			"%.2lf,%.2lf,%.2lf"
			",",
			// time stamp
			str1,
			// angular speed
			p,q,r
			);
	strcpy(s,str);
}

void AngularVelocity::sread(char *s) {
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
			" %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// angular speed
			&p,&q,&r
			);
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}


bool AngularVelocity::fread(FILE *f) {
	char str[64];
    // angular speed
	fscanf(f,"%s %lf",str,&p);
    if(strcmp(str,"p")!=0)
    {
        printf("AngularVelocity::fread p expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&q);
    if(strcmp(str,"q")!=0)
    {
        printf("AngularVelocity::fread q expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&r);
    if(strcmp(str,"r")!=0)
    {
        printf("AngularVelocity::fread r expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	return true;
}

