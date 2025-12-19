/*
 * ModemRangeHeadingEkf.h
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#ifndef MODEMRANGEHEADINGEKF_H_
#define MODEMRANGEHEADINGEKF_H_

#include <stdio.h>

#include <inttypes.h>

#include "AngularPosition.h"
#include "RangeModem.h"
#include "LinearVelocity.h"
#include "LinearPosition.h"

#include "Matrix.h"

#include <string>
using namespace std;

class ModemRangeHeadingEkf
{
private:
	double *mTimeStamp;

	string mName;
	// input: estimated heading , estimated Leader position and range measurements from Leader provided by acoustic modem
	// input: estimated linear velocity with respect to the water
	AngularPosition *mAngPosHat;
	RangeModem *mRangeModem;
	LinearVelocity *mLinVelHat;
	// output: estimates
	LinearPosition *mLinPosHat;

	double mDt;

	bool mInitFlag;

public:
	// task constructor
	ModemRangeHeadingEkf(double dt);
	~ModemRangeHeadingEkf(void) {};

	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
	void set_name(string name) { mName=name; }

	void set_input_output(AngularPosition *angPosHat,RangeModem *rangeModem,
			LinearVelocity *linVelHat,LinearPosition *linPosHat);

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
	double sigmaRo;

private:
	// state transition matrix: 6x6
	Matrix F;
	// control-input matrix: 6x2
	Matrix B;
	// control vector: 2x1
	Matrix u;
	// estimated state and covariance: 6x1 , 6x6
	Matrix x,P;
	// predicted  state and covariance: 6x1 , 6x6
	Matrix xPred,Ppred;
	// system and measurement noise covariance: 6x6 ; 3x3
	Matrix Q, R;
	// measurement, expected measurement, and innovation vector: 3x1 ; 3x1 ; 3x1
	Matrix z,zHat,y;
	// observation matrix: 3x6
	Matrix H;
	// innovation covariance: 3x3
	Matrix S;
	// Kalman gain: 6x3
	Matrix K;

private:
	void init_filter(void);
};


#endif /* MODEMRANGEHEADINGEKF_H_ */
