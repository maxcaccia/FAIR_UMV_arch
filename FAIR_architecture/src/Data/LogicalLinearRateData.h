/*
 * LogicalLinearRateData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef LOGICALLINEARRATEDATA_H_
#define LOGICALLINEARRATEDATA_H_

#include "BodyLinearRateData.h"
#include "DevicePose.h"

#define LOGICAL_LINEAR_RATE_CODE "LOGICAL_LIN_VEL"

class LogicalLinearRateData : public BodyLinearRateData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	LogicalLinearRateData(string name);
	LogicalLinearRateData(void);
	LogicalLinearRateData(const LogicalLinearRateData& obj) { *this = obj; }
    ~LogicalLinearRateData(void) {};

    LogicalLinearRateData & operator = (const LogicalLinearRateData &obj);
    void assign(BodyLinearRateData bodyLinearRateData);
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

#endif /* LOGICALLINEARRATEDATA_H_ */
