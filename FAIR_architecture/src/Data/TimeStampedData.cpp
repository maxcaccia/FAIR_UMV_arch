/*
 * TimeStampedData.cpp
 *
 *  Created on: Dec 7, 2022
 *      Author: mc
 */

#include "TimeStampedData.h"

TimeStampedData & TimeStampedData::operator = (const TimeStampedData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	mPacketCode=obj.mPacketCode;
	mName=obj.mName;
	mTimeStamp=obj.mTimeStamp;
	mValid=obj.mValid;
	mUpdated=obj.mUpdated;

	// return a reference to myself
	return *this;
}

bool TimeStampedData::fprint_csv_log_name(FILE *f) {
	if(f==NULL) return false;
	char str[2048];
	sprint_csv_log_name(str);
	fprintf(f,"%s",str);
	return true;
}

bool TimeStampedData::fprint_csv_long_name(FILE *f) {
	if(f==NULL) return false;
	char str[2048];
	sprint_csv_long_name(str);
	fprintf(f,"%s",str);
	return true;
}

void TimeStampedData::sprint_csv_log_name(char *s) {
	char str[2048];
	sprintf(str,"%s_TimeStamp,%s_valid_flag,",mName.c_str(),mName.c_str());
	strcpy(s,str);
}

void TimeStampedData::sprint_csv_long_name(char *s) {
	char str[2048];
	sprintf(str,"time,,");  // robot telemetry time stamp
	strcpy(s,str);
}


void TimeStampedData::sprint_csv(char *s) {
	sprintf(s,"%.3lf,%" PRIu8 ",",mTimeStamp,(uint8_t)mValid);
}


int TimeStampedData::sprint(char *s) {
	int n=sprintf(s,"%s %s %.3lf %" PRIu8 " ",mPacketCode.c_str(),mName.c_str(),mTimeStamp,(uint8_t)mValid);
	return n;
}

int TimeStampedData::sprint_cr_lf(char *s) {
	char str[2048];
	sprint(str);
	int n=sprintf(s,"%s\r\n",str);
	return n;
}

void TimeStampedData::sread(char *s) {
	char packetCode[64];
	char name[64];
	uint8_t valid;
	sscanf(s,"%s %s %lf %" SCNu8,packetCode,name,&mTimeStamp,&valid);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}



bool TimeStampedData::fprint(FILE *f) {
	if(f==NULL) return false;
	char str[2048];
	sprint(str);
	fprintf(f,"%s ",str);
	return true;
}

bool TimeStampedData::fprint_csv(FILE *f) {
	if(f==NULL) return false;
	char str[256];
	sprint_csv(str);
	fprintf(f,"%s,",str);
	return true;
}


void get_time_with_ms(time_t &s,int32_t &ms) {
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999) {
        s++;
        ms = 0;
    }
}


