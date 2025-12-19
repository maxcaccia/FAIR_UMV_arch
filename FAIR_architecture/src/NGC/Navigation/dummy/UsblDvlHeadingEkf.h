/*
 * UsblDvlHeadingEkf.h
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#ifndef USBLDVLHEADINGEKF_H_
#define USBLDVLHEADINGEKF_H_

#include <stdio.h>

#include <inttypes.h>

#include "AngularPosition.h"
#include "UsblData.h"
#include "DvlData.h"
#include "LinearPosition.h"

#include "Matrix.h"

#include <string>
using namespace std;

class UsblDvlHeadingEkf
{
private:
	double *mTimeStamp;

	string mName;
	// input: measured USBL position , estimated linear velocity, heading
	AngularPosition *mAngPosHat; // estimated heading and attitude
	UsblData *mUsblMeas; // USBL measurements (already converted in the local NED frame)
	DvlData *mDvlMeas; // DVL velocity measurements
	// output: estimates
	LinearPosition *mLinPosHat; // estimated position

	double mDt;

	bool mInitFlag;

public:
	// task constructor
	UsblDvlHeadingEkf(double dt);
	~UsblDvlHeadingEkf(void) {};

	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
	void set_name(string name) { mName=name; }

	void set_input_output(AngularPosition *angPosHat,UsblData *usblMeas,
			DvlData *dvlMeas,LinearPosition *linPosHat);

	bool read_configuration(string filename);

	void execute(void);
	void reset(void);

	void print(void);

private:
	//
	// position uncertainty (init)
	//
	double dx0;

	double sigmaXdot;
	double roP;
	double roV;

private:
	// state transition matrix: 4x4
	Matrix F;
	// estimated state and covariance: 4x1 , 4x4
	Matrix x,P;
	// predicted  state and covariance: 4x1 , 4x4
	Matrix xPred,Ppred;
	// system and measurement noise covariance: 4x4 ; 2x2 / 4x4
	Matrix Q, R2p, R2v, R4;
	// measurement, expected measurement, and innovation vector: 2x1 / 4x1 ; 2x1 / 4x1 ; 2x1 / 4x1
	Matrix z2,z4, z2Hat,z4Hat, y2,y4;
	// observation matrix:  2x4 , 4x4
	Matrix H2, H4;
	// innovation covariance:  2x2 / 4x4
	Matrix S2, S4;
	// Kalman gain: 4x2 / 4x4
	Matrix K2, K4;

private:
	void init_filter(void);
};


#endif /* USBLDVLHEADINGEKF_H_ */
