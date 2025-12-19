/*
 * VehicleDvlData.h
 *
 *  Created on: 13 January 2025
 *      Author: massimo
 */
#ifndef VEHICLEDVLDATA_H_
#define VEHICLEDVLDATA_H_

#include "LogicalDvlData.h"
#include "DevicePose.h"

#define VEHICLE_DVL_CODE "VEHICLE_DVL"

class VehicleDvlData : public LogicalDvlData {
public:
	// device pose in <B>
	DevicePose mPose;

public:
	VehicleDvlData(string name);
	VehicleDvlData(void);
	VehicleDvlData(const VehicleDvlData& obj) { *this = obj; }
    ~VehicleDvlData(void) {};

    VehicleDvlData & operator = (const VehicleDvlData &obj);
    void assign(LogicalDvlData logicalDvlData);
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

#endif /* VEHICLEDVLDATA_H_ */
