/*
 * LogicalSpeedData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef LOGICALSPEEDDATA_H_
#define LOGICALSPEEDDATA_H_

#include "BodySpeedData.h"
#include "DevicePose.h"

#define LOGICAL_SPEED_CODE "LOGICAL_SPEED"

class LogicalSpeedData : public BodySpeedData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	LogicalSpeedData(string name);
	LogicalSpeedData(void);
	LogicalSpeedData(const LogicalSpeedData& obj) { *this = obj; }
    ~LogicalSpeedData(void) {};

    LogicalSpeedData & operator = (const LogicalSpeedData &obj);
    void assign(BodySpeedData bodySpeedData);
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

#endif /* LOGICALSPEEDDATA_H_ */
