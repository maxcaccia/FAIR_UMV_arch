/*
 * LinearVelocityPredictor.h
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#ifndef LINEARVELOCITYPREDICTOR_H_
#define LINEARVELOCITYPREDICTOR_H_

#include <stdio.h>

#include <inttypes.h>

#include "ForceMoment.h"
#include "AngularVelocity.h"
#include "LinearVelocity.h"
#include "HydroDynamicsCntrl.h"

#include <string>
using namespace std;

class LinearVelocityPredictor
{
private:
	string mName;
	// input: forces, angular velocity
	ForceMoment *mForceMoment;
	AngularVelocity *mAngVelHat;
	// output: estimates
	LinearVelocity *mLinVelHat;
	// parameters: hydrodynamics
	HydroDynamicsCntrl* mHydroPar;

	double mDt;
	double *mTimeStamp;

public:
	// task constructor
	LinearVelocityPredictor(double dt);
	~LinearVelocityPredictor(void);

	void set_name(string name) { mName=name; }
	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }

	void set_input_output(ForceMoment *forceMoment,AngularVelocity *angVelHat,LinearVelocity *linVelHat,HydroDynamicsCntrl *hydroPar);

//	bool read_configuration(string fileName);

	void execute(void);
	void reset(void);

	void print(void);

private:
	double mu;
	double Xu;
	double Xuu;
	double mv;
	double Yv;
	double Yvv;
	double mxg;
};


#endif /* LINEARVELOCITYPREDICTOR_H_ */
