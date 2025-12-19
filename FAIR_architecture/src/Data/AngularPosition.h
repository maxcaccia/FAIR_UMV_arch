/*
 * AngularPosition.h
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
#ifndef ANGULARPOSITION_H_
#define ANGULARPOSITION_H_

#include "TimeStampedData.h"
#include "nVector.h"

#define ANGULAR_POSITION_CODE "ANG_POS"

class AngularPosition : public TimeStampedData {
public:
	// angles
	double phi; // roll [deg]
	double theta; // pitch [deg]
	double psi; // yaw [deg]

public:
	AngularPosition(string name);
	AngularPosition(void);
	AngularPosition(const AngularPosition& obj) { *this = obj; }
    ~AngularPosition(void) {};

    AngularPosition & operator = (const AngularPosition &obj);

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

#endif /* KINEMATICS_H_ */
