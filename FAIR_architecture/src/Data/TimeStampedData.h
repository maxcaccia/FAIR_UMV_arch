/*
 * TimeStampedData.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
#ifndef TIMESTAMPEDDATA_H_
#define TIMESTAMPEDDATA_H_

#include <mutex>

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <string>
using namespace std;

#define TIME_STAMP_CODE "TIME_STAMP"

class TimeStampedData {
public:
	string mPacketCode;
	string mName;
//	uint64_t mTimeStamp; // robot telemetry time stamp: to be discussed with Marco
	double mTimeStamp; // hhmmss.sss in UTC (ISO 8601 standard)
	bool mValid;
	bool mUpdated;
	std::mutex mMutex; // mutex for safe access: initialised unlocked

public:
	TimeStampedData(string name) { mPacketCode=TIME_STAMP_CODE; mName=name; mTimeStamp=0; mValid=false; mUpdated=false; mMutex.unlock(); }
	TimeStampedData(void) { mPacketCode=TIME_STAMP_CODE; mName=""; mTimeStamp=0; mValid=false; mUpdated=false; mMutex.unlock(); }
    virtual ~TimeStampedData(void) {};

    void set_time_stamp(uint64_t timeStamp) { mTimeStamp=timeStamp; mUpdated=true; }
    void set_expired(void) { mUpdated=false; }
    void set_name(string name) {mName=name;}

    void lock(void) { mMutex.lock(); }
    void unlock(void) { mMutex.unlock(); }

    TimeStampedData & operator = (const TimeStampedData &obj);

    // print headers
    bool fprint_csv_log_name(FILE *f);
    bool fprint_csv_long_name(FILE *f);
    virtual void sprint_csv_log_name(char *s);
    virtual void sprint_csv_long_name(char *s);
    // print data
    bool fprint(FILE *f);
    bool fprint_csv(FILE *f);
	virtual int sprint(char *s);
	int sprint_cr_lf(char *s);
	virtual void sread(char *s);
	virtual void sprint_csv(char *s);
	// publish MQTT topics
	virtual void mqtt_publish(void) { printf("TimeStampedData::mqtt_publish(char *s)\n"); }
	// read data from file
    virtual bool fread(string fileName)  { printf("TimeStampedData::fread(string fileName) %s\n",fileName.c_str()); return false; }


private:
	virtual void reset(void) { printf("TimeStampedData::reset(char *s)\n"); }
};

void get_time_with_ms(time_t &s,int32_t &ms);

#endif /* TIMESTAMPEDDATA_H_ */
