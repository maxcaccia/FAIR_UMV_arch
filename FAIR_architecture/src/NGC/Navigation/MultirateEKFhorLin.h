/*
 * MultirateEKFhorLin.h
 *
 *  Created on: 24 feb 2025
 *      Author: massimo
 */

#ifndef MULTIRATEEKFHORLIN_H_
#define MULTIRATEEKFHORLIN_H_

#include <stdio.h>

#include <inttypes.h>

#include "LogicalPositionData.h"
#include "LogicalSpeedData.h"
#include "LogicalLinearRateData.h"
#include "LogicalAttitudeData.h"
#include "LogicalAngularRateData.h"
#include "SeaCurrent.h"
#include "Matrix.h"

#include <string>
using namespace std;

class MultirateEKFhorLin
{
private:
	double *mTimeStamp;

	string mName;

    //
    // NGC: local NED frame latitude and longitude
    //
    double *lat0;
    double *lon0;

	//
	// input: measured latitude-longitude, NED speed and velocity in the body-fixed reference frame
	//
	LogicalPositionData *mPosition;
	LogicalSpeedData *mSpeedNED;
	LogicalLinearRateData *mBodyVelocity;
	//
	// input: estimated attitude and attitude rate
	//
	LogicalAttitudeData *mPhiThetaPsiHat; // estimated roll, pitch, yaw
	LogicalAngularRateData *mpqrHat; // estimated roll, pitch, yaw rate
	//
	// output: estimates
	//
	LogicalPositionData *mPositionHat;
	LogicalLinearRateData *mBodyVelocityWrtWaterHat;
	SeaCurrent *mSeaCurrentHat;

	double mDt;

	bool mInitFlag;

public:
	// task constructor
	MultirateEKFhorLin(double dt);
	~MultirateEKFhorLin(void) {};

	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
	void set_name(string name) { mName=name; }

	void set_local_NED_frame(double *lat0,double *lon0)
		{ this->lat0=lat0; this->lon0=lon0; }
	void set_input_output(LogicalPositionData *position,
			LogicalSpeedData *speedNED,LogicalLinearRateData *bodyVelocity,
			LogicalAttitudeData *phiThetaPsiHat,LogicalAngularRateData *pqrHat,
			LogicalPositionData *positionHat,
			LogicalLinearRateData *bodyVelocityWrtWaterHat,SeaCurrent *seaCurrentHat) {
		mPosition=position; mSpeedNED=speedNED; mBodyVelocity=bodyVelocity;
		mPhiThetaPsiHat=phiThetaPsiHat; mpqrHat=pqrHat;
		mPositionHat=positionHat;
		mBodyVelocityWrtWaterHat=bodyVelocityWrtWaterHat; mSeaCurrentHat=seaCurrentHat;
	}

	bool read_configuration(string filename);

	void execute(void);
	void reset(void);

	void print(void);

private:
	//
	// latitude-longitude, NED speed and velocity uncertainty (init)
	//
	double sigma_lat_lon0;
	double sigma_ur_vr0;
	double sigma_sea_current0;

	//
	// system noise (acceleration)
	//
	double sigma_urvrDot;
	double sigma_seaCurrentDot;

private:
	// state transition matrix: 6x6
	Matrix F;
	// control-input matrix: 6x2
	Matrix B;
	// control vector: 2x1
	Matrix uCntrl;
	// estimated state and covariance: 6x1 , 6x6
	Matrix x,P;
	// predicted  state and covariance: 6x1 , 6x6
	Matrix xPred,Ppred;
	// system noise covariance: 6x6
	Matrix Q;
	// measurement noise covariance: 6x6
	Matrix R;
	// measurement, expected measurement, and innovation vector: 6x1 ; 6x1 ; 6x1
	Matrix z,zHat,y;
	// observation matrix: 6x6
	Matrix H;
	// innovation covariance: 6x6
	Matrix S;
	// Kalman gain: 6x6
	Matrix K;
	// identity matrix: 6x6
	Matrix I;


	double mu,mv,Xu,Xuu,Xv,Xvv,mr2;

private:
	void init_filter(void);

};


#endif /* MULTIRATEEKFHORLIN_H_ */
