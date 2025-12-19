/*
 * Lkf2ndOrder180.h
 *
 *  Created on: 14 February 2025
 *      Author: massimo
 */

#ifndef LKF2NDORDER180_H_
#define LKF2NDORDER180_H_

#include <stdio.h>

#include <inttypes.h>

#include "Matrix.h"

#include <string>
using namespace std;

class Lkf2ndOrder180
{
private:
	double *mTimeStamp;

	string mName;

	// input: measured position, measured velocity
	double *x; // measured position
	double *xDot; // measured velocity
	bool *valid; // valid measurement flag
	//
	// measurement noise: position and velocity
	//
	double *rP;
	double *rV;
	// output: estimates
	double *xHat; // estimated position
	double *xDotHat; // estimated velocity
	bool *xValidHat; // valid estimate flag
	bool *xDotValidHat; // valid estimate flag
	double *Pxx; // covariance of estimated position
	double *PxDotxDot; // covariance of estimated velocity

	double mDt;

	bool mInitFlag;

public:
	// task constructor
	Lkf2ndOrder180(double dt);
	~Lkf2ndOrder180(void) {};

	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
	void set_name(string name) { mName=name; }

	void set_input_output(double *x,double *xDot,bool *valid,
			double *xHat,double *xDotHat,bool *xValidHat,bool *xDotValidHat,double *Pxx,double *PxDotxDot) {
		this->x=x; this->xDot=xDot; this->valid=valid;
		this->xHat=xHat; this->xDotHat=xDotHat;
		this->xValidHat=xValidHat;
		this->xDotValidHat=xDotValidHat;
		this->Pxx=Pxx; this->PxDotxDot=PxDotxDot;
	}
	bool read_configuration(string filename);

	void execute(void);
	void reset(void);

	void init_filter(void);

	void print(void) { printf("Lkf2ndOrder180::print %s\n",mName.c_str());fflush(stdout); }

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
	// state transition matrix: 2x2
	Matrix F;
	// estimated state and covariance: 2x1 , 2x2
	Matrix xState,P;
	// predicted  state and covariance: 2x1 , 2x2
	Matrix xPred,Ppred;
	// system and measurement noise covariance: 2x2 , 2x2
	Matrix Q, R;
	// measurement, expected measurement, and innovation vector: 2x1 , 2x1 , 2x1
	Matrix z, zHat, y;
	// observation matrix:  2x2
	Matrix H;
	// innovation covariance:  2x2
	Matrix S;
	// Kalman gain: 2x2
	Matrix K;
	// identity matrix: 2x2
	Matrix I;
};


#endif /* LKF2NDORDER180_H_ */
