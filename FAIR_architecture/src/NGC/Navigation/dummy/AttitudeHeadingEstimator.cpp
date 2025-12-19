/*
 * AttitudeHeadingEstimator.cpp
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "AttitudeHeadingEstimator.h"


AttitudeHeadingEstimator::AttitudeHeadingEstimator(void) {
	mTimeStamp=NULL;
	mAttitudeData=NULL;
	mAngularRateData=NULL;
	mAttitudeHat=NULL;
	mAngularRateHat=NULL;
};


void AttitudeHeadingEstimator::execute(void)
{
	printf("AttitudeHeadingEstimator::execute mTimeStamp %.3lf\n",*mTimeStamp);fflush(stdout);
	//
	// check available measurements
	//
	if(mAttitudeData->mTimeStamp == *mTimeStamp) {
		mAttitudeHat->phi = mAttitudeData->phi;
		mAttitudeHat->theta = mAttitudeData->theta;
		mAttitudeHat->psi = mAttitudeData->psi;
		mAttitudeHat->mValid = mAttitudeData->mValid;
	}
	if(mAngularRateData->mTimeStamp == *mTimeStamp) {
		mAngularRateHat->p = mAngularRateData->p;
		mAngularRateHat->q = mAngularRateData->q;
		mAngularRateHat->r = mAngularRateData->r;
		mAngularRateHat->mValid = mAngularRateData->mValid;
	}

}


void AttitudeHeadingEstimator::set_input_output(LogicalAttitudeData *attitudeData,LogicalAngularRateData *angularRateData,
		LogicalAttitudeData *attitudeHat,LogicalAngularRateData *angularRateHat) {
	mAttitudeData=attitudeData;
	mAngularRateData=angularRateData;
	mAttitudeHat=attitudeHat;
	mAngularRateHat=angularRateHat;
}


void AttitudeHeadingEstimator::reset(void)
{
	mAttitudeHat->psi=0.;
	mAttitudeHat->theta=0.;
	mAttitudeHat->phi=0.;
	mAngularRateHat->p=0.;
	mAngularRateHat->q=0.;
	mAngularRateHat->r=0.;
	mAttitudeHat->mValid = mAngularRateHat->mValid = false;
}

void AttitudeHeadingEstimator::print(void) {
	printf("AttitudeHeadingEstimator::print %s\n",mName.c_str());
}



