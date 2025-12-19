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
	mName=obj.mName;
	mTimeStamp=obj.mTimeStamp;
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
	sprintf(str,",%s_TimeStamp,",mName.c_str());
	strcpy(s,str);
}

void TimeStampedData::sprint_csv_long_name(char *s) {
	char str[2048];
	sprintf(str,",,");  // robot telemetry time stamp
	strcpy(s,str);
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


