/*
 * AngularVelocity.h
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
#ifndef ANGULARVELOCITY_H_
#define ANGULARVELOCITY_H_

#include "TimeStampedData.h"
#include "nVector.h"

#define ANGULAR_VELOCITY_CODE "ANG_VEL"

class AngularVelocity : public TimeStampedData {
public:
	// angular velocity
	double p; // roll rate [deg/s]
	double q; // pitch rate [deg/s]
	double r; // yaw rate [deg/s]

public:
	AngularVelocity(string name);
	AngularVelocity(void);
	AngularVelocity(const AngularVelocity& obj) { *this = obj; }
    ~AngularVelocity(void) {};

    AngularVelocity & operator = (const AngularVelocity &obj);

    bool fread(FILE *f);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

private:
	void reset(void);

};

#endif /* ANGULARVELOCITY_H_ */
