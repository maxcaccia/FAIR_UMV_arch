/*
 * TimeStampedData.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
#ifndef TIMESTAMPEDDATA_H_
#define TIMESTAMPEDDATA_H_

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <string>
using namespace std;

class TimeStampedData {
public:
	string mName;
	uint64_t mTimeStamp; // robot telemetry time stamp: to be discussed with Marco

public:
	TimeStampedData(string name) { mName=name; mTimeStamp=0; }
	TimeStampedData(void) { mName=""; mTimeStamp=0; }
    virtual ~TimeStampedData(void) {};

    void set_time_stamp(uint64_t timeStamp) { mTimeStamp=timeStamp; }
    void set_name(string name) {mName=name;}

    TimeStampedData & operator = (const TimeStampedData &obj);

    // print headers
    bool fprint_csv_log_name(FILE *f);
    bool fprint_csv_long_name(FILE *f);
    virtual void sprint_csv_log_name(char *s);
    virtual void sprint_csv_long_name(char *s);
    // print data
    bool fprint(FILE *f);
    bool fprint_csv(FILE *f);
	virtual int sprint(char *s) { int n=sprintf(s,"%s %" PRIu64 " ",mName.c_str(),mTimeStamp); return n;}
	virtual void sread(char *s) { char name[64];sscanf(s,"%s %" SCNu64,name,&mTimeStamp);mName=name;}
	virtual void sprint_csv(char *s) { sprintf(s,"%s,%" PRIu64 ",",mName.c_str(),mTimeStamp); }
	// publish MQTT topics
	virtual void mqtt_publish(void) { printf("TimeStampedData::mqtt_publish(char *s)\n"); }
	// read data from file
    virtual bool fread(string fileName)  { printf("TimeStampedData::fread(string fileName) %s\n",fileName.c_str()); return false; }


private:
	virtual void reset(void) { printf("TimeStampedData::reset(char *s)\n"); }
};

void get_time_with_ms(time_t &s,int32_t &ms);

#endif /* TIMESTAMPEDDATA_H_ */
