/*
 * LogicalAngularRateData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef LOGICALANGULARRATEDATA_H_
#define LOGICALANGULARRATEDATA_H_

#include "BodyAngularRateData.h"
#include "DevicePose.h"

#define LOGICAL_ANGULAR_RATE_CODE "LOGICAL_ANG_VEL"

class LogicalAngularRateData : public BodyAngularRateData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	LogicalAngularRateData(string name);
	LogicalAngularRateData(void);
	LogicalAngularRateData(const LogicalAngularRateData& obj) { *this = obj; }
    ~LogicalAngularRateData(void) {};

    LogicalAngularRateData & operator = (const LogicalAngularRateData &obj);
    void assign(BodyAngularRateData bodyAngularRateData);
    void assign(DevicePose devicePose);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
	void sprint_csv(char *s);
	// read data
	void sread(char *s);

private:
	void reset(void);
};

#endif /* LOGICALANGULARRATEDATA_H_ */
