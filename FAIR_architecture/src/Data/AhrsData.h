/*
 * AHRSdata.h
 *
 *  Created on: 22 October 2023
 *      Author: massimo
 */
#ifndef AHRSDATA_H_
#define AHRSDATA_H_

#include "TimeStampedData.h"

#define AHRS_CODE "AHRS"

#define IMU_TEMPERATURE 37.0

class AhrsData : public TimeStampedData {
public:
	// angles
	double phi; // roll [deg]
	double theta; // pitch [deg]
	double psi; // yaw [deg]
	// linear accelerations
	double uDot; // [m/s^2]
	double vDot; // [m/s^2]
	double wDot; // [m/s^2]
	// angular velocity
	double p; // roll rate [deg/s]
	double q; // pitch rate [deg/s]
	double r; // yaw rate [deg/s]
	// magnetic components
	double magnX; // [Gauss]
	double magnY; // [Gauss]
	double magnZ; // [Gauss]
	// linear velocity
	double u; // [m/s]
	double v; // [m/s]
	double w; // [m/s]

public:
	AhrsData(string name);
	AhrsData(void);
	AhrsData(const AhrsData& obj) { *this = obj; }
    ~AhrsData(void) {};

    AhrsData & operator = (const AhrsData &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
	void sprint_csv(char *s);
	// read data
	void sread(char *s);

protected:
	void reset(void);
};

#endif /* AHRSDATA_H_ */
