/*
 * VehicleGnssVelocityData.h
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
#ifndef VEHICLEGNSSVELOCITYDATA_H_
#define VEHICLEGNSSVELOCITYDATA_H_

#include "LogicalGnssVelocityData.h"
#include "DevicePose.h"

#define VEHICLE_GNSS_VELOCITY_CODE "VEHICLE_GNSS_VEL"

class VehicleGnssVelocityData : public LogicalGnssVelocityData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	VehicleGnssVelocityData(string name);
	VehicleGnssVelocityData(void);
	VehicleGnssVelocityData(const VehicleGnssVelocityData& obj) { *this = obj; }
    ~VehicleGnssVelocityData(void) {};

    VehicleGnssVelocityData & operator = (const VehicleGnssVelocityData &obj);
    void assign(LogicalGnssVelocityData logicalGnssVelocityData);
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

#endif /* VEHICLEGNSSVELOCITYDATA_H_ */
