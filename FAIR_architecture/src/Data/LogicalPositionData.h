/*
 * LogicalPositionData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef LOGICALPOSITIONDATA_H_
#define LOGICALPOSITIONDATA_H_

#include "BodyPositionData.h"
#include "DevicePose.h"

#define LOGICAL_POSITION_CODE "LOGICAL_POS"

class LogicalPositionData : public BodyPositionData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	LogicalPositionData(string name);
	LogicalPositionData(void);
	LogicalPositionData(const LogicalPositionData& obj) { *this = obj; }
    ~LogicalPositionData(void) {};

    LogicalPositionData & operator = (const LogicalPositionData &obj);
    void assign(BodyPositionData bodyPositionData);
    void assign(DevicePose devicePose);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
	void sprint_csv(char *s);
	// read data
	void sread(char *s);

//	void compute_rigid_body_position_planar(BodyPositionData &rb,double psi);

private:
	void reset(void);
};

#endif /* LOGICALPOSITIONDATA_H_ */
