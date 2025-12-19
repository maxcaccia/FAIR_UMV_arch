/*
 * LogicalAhrsData.h
 *
 *  Created on: 9 January 2025
 *      Author: massimo
 */
#ifndef LOGICALAHRSDATA_H_
#define LOGICALAHRSDATA_H_

#include "AhrsData.h"

#define LOGICAL_AHRS_CODE "LOGICAL_AHRS"

class LogicalAhrsData : public AhrsData {
public:
	// angles
	double phiStd; // roll [deg]
	double thetaStd; // pitch [deg]
	double psiStd; // yaw [deg]
//	// linear accelerations
//	double uDot; // [m/s^2]
//	double vDot; // [m/s^2]
//	double wDot; // [m/s^2]
	// angular velocity
	double pStd; // roll rate [deg/s]
	double qStd; // pitch rate [deg/s]
	double rStd; // yaw rate [deg/s]
//	// magnetic components
//	double magnX; // [Gauss]
//	double magnY; // [Gauss]
//	double magnZ; // [Gauss]
//	// linear velocity
//	double u; // [m/s]
//	double v; // [m/s]
//	double w; // [m/s]

public:
	LogicalAhrsData(string name);
	LogicalAhrsData(void);
	LogicalAhrsData(const LogicalAhrsData& obj) { *this = obj; }
    ~LogicalAhrsData(void) {};

    LogicalAhrsData & operator = (const LogicalAhrsData &obj);
    void assign(AhrsData ahrsData);

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

#endif /* LOGICALAHRSDATA_H_ */
