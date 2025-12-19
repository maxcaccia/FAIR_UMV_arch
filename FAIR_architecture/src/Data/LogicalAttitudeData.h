/*
 * LogicalAttitudeData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef VEHICLEAHRSDATA_H_
#define VEHICLEAHRSDATA_H_

#include "BodyAttitudeData.h"
#include "DevicePose.h"

#define LOGICAL_ATTITUDE_CODE "LOGICAL_ATT"

class LogicalAttitudeData : public BodyAttitudeData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	LogicalAttitudeData(string name);
	LogicalAttitudeData(void);
	LogicalAttitudeData(const LogicalAttitudeData& obj) { *this = obj; }
    ~LogicalAttitudeData(void) {};

    LogicalAttitudeData & operator = (const LogicalAttitudeData &obj);
    void assign(BodyAttitudeData bodyAttitudeData);
    void assign(DevicePose devicePose);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
	void sprint_csv(char *s);
	// read data
	void sread(char *s);

//	void compute_rigid_body_attitude(BodyAttitudeData &rb);

private:
	void reset(void);
};

#endif /* VEHICLEAHRSDATA_H_ */
