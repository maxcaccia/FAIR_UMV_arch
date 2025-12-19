/*
 * LinearVelocity.h
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
#ifndef LINEARVELOCITY_H_
#define LINEARVELOCITY_H_

#include "TimeStampedData.h"
#include "nVector.h"
#include "AngularPosition.h"

#define LINEAR_VELOCITY_CODE "LIN_VEL"

class LinearVelocity : public TimeStampedData {
public:
	// linear velocity and speed (NED)
	double xDot; // North speed [m/s]
	double yDot; // East speed [m/s]
	double zDot; // Downward speed [m/s]
	double groundSpeed; // [m/s]
	double course; // [deg]
	// sea current
	double xCdot; // [m/s]
	double yCdot; // [m/s]
	double zCdot; // [m/s]
	// linear velocity in vehicle-fixed frame
	double u; // [m/s]
	double v; // [m/s]
	double w; // [m/s]
	// linear velocity with respect to the water in vehicle-fixed frame
	double ur; // [m/s]
	double vr; // [m/s]
	double wr; // [m/s]

public:
	LinearVelocity(string name);
	LinearVelocity(void);
	LinearVelocity(const LinearVelocity& obj) { *this = obj; }
    ~LinearVelocity(void) {};

    LinearVelocity & operator = (const LinearVelocity &obj);

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

public:
	AngularPosition *mAttitude;
	void set_attitude(AngularPosition *attitude) { mAttitude=attitude; }

public:
	void compute_speed_from_velocity(void);
	void compute_NED_velocity_speed_from_body_relative_speed_and_sea_current(AngularPosition *attitude);
	void compute_body_velocity_from_NED_velocity(AngularPosition *attitude);
};

#endif /* LINEARVELOCITY_H_ */
