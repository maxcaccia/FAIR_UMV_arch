/*
 * GnssEkf.h
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#ifndef GNSSEKF_H_
#define GNSSEKF_H_

#include <stdio.h>

#include <inttypes.h>

#include "AngularPosition.h"
#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "LinearVelocity.h"
#include "LinearPosition.h"

#include "Matrix.h"

#include <string>
using namespace std;

class GnssEkf
{
private:
	double *mTimeStamp;

	string mName;
	// input: yaw motion , linear velocity with respect to the water, gnss measurements
	AngularPosition *mAngPosHat;
	GnssPositionData *mGnssGGA;
	GnssVelocityData *mGnssVTG;
	// input/output: estimates
	LinearVelocity *mLinVelHat;
	// output: estimates
	LinearPosition *mLinPosHat;

	double mDt;

	bool mInitFlag;

public:
	// task constructor
	GnssEkf(double dt);
	~GnssEkf(void) {};

	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
	void set_name(string name) { mName=name; }

	void set_input_output(AngularPosition *angPosHat,LinearVelocity *linVelHat,
			GnssPositionData *gnssGGA,GnssVelocityData *gnssVTG,
			LinearPosition *linPosHat);

	bool read_configuration(string filename);

	void execute(void);
	void reset(void);

	void print(void);

private:
	//
	// position and current uncertainty (init)
	//
	double dx0,dxDotC0;

	double sigmaXdot;
	double sigmaCurrentDot;
	double roX;
	double roU;
	double roChi;

private:
	// state transition matrix: 4x4
	Matrix F;
	// control-input matrix: 4x2
	Matrix B;
	// control vector: 2x1
	Matrix u;
	// estimated state and covariance: 4x1 , 4x4
	Matrix x,P;
	// predicted  state and covariance: 4x1 , 4x4
	Matrix xPred,Ppred;
	// system and measurement noise covariance: 4x4 ; 2x2 / 4x4
	Matrix Q, R2xy,R2Uchi,R4;
	// measurement, expected measurement, and innovation vector: 2x1 / 4x1 ; 2x1 / 4x1
	Matrix z2,z4,zHat2,zHat4,y2,y4;
	// observation matrix: 2x4 , 4x4
	Matrix H2, H4;
	// innovation covariance: 2x2 / 4x4
	Matrix S2,S4;
	// Kalman gain: 4x2 / 4x4
	Matrix K2,K4;

private:
	void init_filter(void);
	void compute_NED_position(double &x,double &y);



};


#endif /* GNSSEKF_H_ */
