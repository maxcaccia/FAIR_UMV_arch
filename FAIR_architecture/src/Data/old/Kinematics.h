/*
 * Kinematics.h
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
#ifndef KINEMATICS_H_
#define KINEMATICS_H_

#include "TimeStampedData.h"
#include "nVector.h"

class Kinematics : public TimeStampedData {
public:
	// latitude, longitude
	double latitude; // [deg.dddddddd]
	double longitude; // [deg.dddddddd]
	// local Earth-fixed NED frame coordinates
	double lat0; // [deg.dddddddd]
	double lon0; // [deg.dddddddd]
	// position in the local Earth-fixed NED frame
	double x; // [m]
	double y; // [m]
	double z; // [m] - z is depth, positive downward
	// angles
	double phi; // roll [deg]
	double theta; // pitch [deg]
	double psi; // yaw [deg]
	// linear velocity and speed (NED)
	double xDot; // North speed [m/s]
	double yDot; // East speed [m/s]
	double zDot; // Downward speed [m/s]
	double speed; // [m/s]
	double groundSpeed; // [m/s]
	double track; // [deg]
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
	// angular velocity
	double p; // roll rate [deg/s]
	double q; // pitch rate [deg/s]
	double r; // yaw rate [deg/s]

public:
	Kinematics(string name);
	Kinematics(void);
    ~Kinematics(void) {};

    Kinematics & operator = (const Kinematics &obj);

    bool fread(string fileName);

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
	void compute_speed_from_velocity(void);
	void compute_NED_velocity_speed_from_body_relative_speed_and_sea_current(void);
	void compute_body_velocity_from_NED_velocity(void);
};

#endif /* KINEMATICS_H_ */
