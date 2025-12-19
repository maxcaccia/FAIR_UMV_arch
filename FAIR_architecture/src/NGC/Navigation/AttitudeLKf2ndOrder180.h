/*
 * AttitudeLKf2ndOrder180.h
 *
 *  Created on: 25 February 2025
 *      Author: massimo
 */

#ifndef ATTITUDELKF2NDORDER180_H_
#define ATTITUDELKF2NDORDER180_H_

#include <stdio.h>

#include <inttypes.h>

#include "LogicalAttitudeData.h"
#include "LogicalAngularRateData.h"
#include "Matrix.h"

#include <string>
using namespace std;

class AttitudeLKf2ndOrder180
{
private:
	double *mTimeStamp;

	string mName;

	// input: measured attitude and angular rate
	LogicalAttitudeData *mPhiThetaPsi; // measured roll, pitch, yaw
	LogicalAngularRateData *mpqr; // measured roll, pitch, yaw rate
	// output: estimated attitude and angular rate
	LogicalAttitudeData *mPhiThetaPsiHat; // estimated roll, pitch, yaw
	LogicalAngularRateData *mpqrHat; // estimated roll, pitch, yaw rate

	double mDt;

	bool mInitFlag;

public:
	// task constructor
	AttitudeLKf2ndOrder180(double dt);
	~AttitudeLKf2ndOrder180(void) {};

	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
	void set_name(string name) { mName=name; }

	void set_input_output(LogicalAttitudeData *phiThetaPsi,
			LogicalAngularRateData *pqr,
			LogicalAttitudeData *phiThetaPsiHat,
			LogicalAngularRateData *pqrHat) {
		mPhiThetaPsi=phiThetaPsi;
		mpqr=pqr;
		mPhiThetaPsiHat=phiThetaPsiHat;
		mpqrHat=pqrHat;
	}

	bool read_configuration(string filename);

	void execute(void);
	void reset(void);

	void init_filter(void);

	void print(void) { printf("AttitudeLKf2ndOrder180::print %s\n",mName.c_str());fflush(stdout); }

private:
	//
	// position and velocity uncertainty (init)
	//
	double sigma_x0;
	double sigma_xDot0;

	//
	// system noise (acceleration)
	//
	double sigma_xDotDot;

private:
	// state transition matrix: 6x6
	Matrix F;
	// estimated state and covariance: 6x1 , 6x6
	Matrix xState,P;
	// predicted  state and covariance: 6x1 , 6x6
	Matrix xPred,Ppred;
	// system and measurement noise covariance: 6x6 , 6x6
	Matrix Q, R;
	// measurement, expected measurement, and innovation vector: 6x1 , 6x1 , 6x1
	Matrix z, zHat, y;
	// observation matrix:  6x6
	Matrix H;
	// innovation covariance:  6x6
	Matrix S;
	// Kalman gain: 6x6
	Matrix K;
	// identity matrix: 6x6
	Matrix I;
};


#endif /* ATTITUDELKF2NDORDER180_H_ */
