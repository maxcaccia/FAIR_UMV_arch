/*
 * VehicleAhrsData.h
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
#ifndef VEHICLEAHRSDATA_H_
#define VEHICLEAHRSDATA_H_

#include "LogicalAhrsData.h"
#include "DevicePose.h"

#define VEHICLE_AHRS_CODE "VEHICLE_AHRS"

class VehicleAhrsData : public LogicalAhrsData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	VehicleAhrsData(string name);
	VehicleAhrsData(void);
	VehicleAhrsData(const VehicleAhrsData& obj) { *this = obj; }
    ~VehicleAhrsData(void) {};

    VehicleAhrsData & operator = (const VehicleAhrsData &obj);
    void assign(LogicalAhrsData logicalAhrsData);
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

#endif /* VEHICLEAHRSDATA_H_ */
