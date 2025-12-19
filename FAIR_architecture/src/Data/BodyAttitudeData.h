/*
 * BodyAttitudeData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef BODYATTITUDEDATA_H_
#define BODYATTITUDEDATA_H_

#include "AhrsData.h"

#define BODY_ATTITUDE_CODE "BODY_ATT"

class BodyAttitudeData : public TimeStampedData {
public:
	// angles
	double phi; // roll [deg]
	double theta; // pitch [deg]
	double psi; // yaw [deg]
	// angles
	double phiStd; // roll [deg]
	double thetaStd; // pitch [deg]
	double psiStd; // yaw [deg]

public:
	BodyAttitudeData(string name);
	BodyAttitudeData(void);
	BodyAttitudeData(const BodyAttitudeData& obj) { *this = obj; }
    ~BodyAttitudeData(void) {};

    BodyAttitudeData & operator = (const BodyAttitudeData &obj);
    void assign(AhrsData ahrsData);
    void assign(double sigmaPhi,double sigmaTheta,double sigmaPsi) {phiStd=sigmaPhi;thetaStd=sigmaTheta;psiStd=sigmaPsi;}

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

#endif /* BODYATTITUDEDATA_H_ */
