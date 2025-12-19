/*
 * VehicleGnssPositionData.h
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
#ifndef VEHICLEGNSSPOSITIONDATA_H_
#define VEHICLEGNSSPOSITIONDATA_H_

#include "LogicalGnssPositionData.h"
#include "DevicePose.h"

#define VEHICLE_GNSS_POSITION_CODE "VEHICLE_GNSS_POS"

class VehicleGnssPositionData : public LogicalGnssPositionData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	VehicleGnssPositionData(string name);
	VehicleGnssPositionData(void);
	VehicleGnssPositionData(const VehicleGnssPositionData& obj) { *this = obj; }
    ~VehicleGnssPositionData(void) {};

    VehicleGnssPositionData & operator = (const VehicleGnssPositionData &obj);
    void assign(LogicalGnssPositionData logicalGnssPositionData);
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

#endif /* VEHICLEGNSSPOSITIONDATA_H_ */
