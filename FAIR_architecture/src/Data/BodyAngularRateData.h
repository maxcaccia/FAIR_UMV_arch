/*
 * BodyAngularRateData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef BODYANGULARRATEDATA_H_
#define BODYANGULARRATEDATA_H_

#include "AhrsData.h"

#define BODY_ANGULAR_RATE_CODE "BODY_ANG_VEL"

class BodyAngularRateData : public TimeStampedData {
public:
	// angular velocity
	double p; // roll rate [deg/s]
	double q; // pitch rate [deg/s]
	double r; // yaw rate [deg/s]
	// angular velocity stdev
	double pStd; // roll rate [deg/s]
	double qStd; // pitch rate [deg/s]
	double rStd; // yaw rate [deg/s]

public:
	BodyAngularRateData(string name);
	BodyAngularRateData(void);
	BodyAngularRateData(const BodyAngularRateData& obj) { *this = obj; }
    ~BodyAngularRateData(void) {};

    BodyAngularRateData & operator = (const BodyAngularRateData &obj);
    void assign(AhrsData ahrsData);
    void assign(double sigmaP,double sigmaQ,double sigmaR) {pStd=sigmaP;qStd=sigmaQ;rStd=sigmaR;}

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

#endif /* BODYANGULARRATEDATA_H_ */
