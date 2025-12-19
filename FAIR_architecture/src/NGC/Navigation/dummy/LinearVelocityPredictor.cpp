/*
 * LinearVelocityPredictor.cpp
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "LinearVelocityPredictor.h"


LinearVelocityPredictor::LinearVelocityPredictor(double dt) {
	mForceMoment=NULL;
	mAngVelHat=NULL;
	mLinVelHat=NULL;
	mHydroPar=NULL;

	mDt=dt;
	mTimeStamp=NULL;

	mu=0.;
	Xu=0.;
	Xuu=0.;
	mv=0.;
	Yv=0.;
	Yvv=0.;
	mxg=0.;
};


void LinearVelocityPredictor::execute(void)
{
	printf("LinearVelocityPredictor::execute mTimeStamp %.3lf\n",*mTimeStamp);fflush(stdout);

	double ur=mLinVelHat->ur;
	double vr=mLinVelHat->vr;
	double r=mAngVelHat->r;
	double X=mForceMoment->X;
	double Y=mForceMoment->Y;
	// surge dynamics
	double murDot=-Xu*ur-Xuu*ur*fabs(ur)+mv*vr*r+mxg*r*r+X;
	double urDot=murDot/mu;
	// sway dynamics
	double mvrDot=-Yv*vr-Yvv*vr*fabs(vr)-mu*ur*r+Y;
	double vrDot=mvrDot/mv;
	// integration step
	ur = ur + urDot*mDt;
	vr = vr + vrDot*mDt;
	// set output
	mLinVelHat->ur = ur;
	mLinVelHat->vr = vr;
	mLinVelHat->mValid = true;
//	mLinVelHat->mTimeStamp = *mTimeStamp;
}


void LinearVelocityPredictor::set_input_output(ForceMoment *forceMoment,AngularVelocity *angVelHat,LinearVelocity *linVelHat,HydroDynamicsCntrl *hydroPar) {
	mForceMoment=forceMoment;
	mAngVelHat=angVelHat;
	mLinVelHat=linVelHat;
	mHydroPar=hydroPar;
	// set hydrodynamics parameters
	mu=mHydroPar->mu;
	Xu=mHydroPar->Xu;
	Xuu=mHydroPar->Xuu;
	mv=mHydroPar->mv;
	Yv=mHydroPar->Yv;
	Yvv=mHydroPar->Yvv;
	mxg=mHydroPar->mxg;
	printf("LinearVelocityPredictor::set_input_output ========================================================\n");
}


void LinearVelocityPredictor::reset(void)
{
	mLinVelHat->ur=0.;
	mLinVelHat->vr=0.;
	mLinVelHat->mValid = false;
}

void LinearVelocityPredictor::print(void) {
	printf("LinearVelocityPredictor::print %s\n",mName.c_str());
}



