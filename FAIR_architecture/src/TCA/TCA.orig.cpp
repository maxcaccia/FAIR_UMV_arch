/*
 * TCA.cpp
 *
 *  Created on: 5 mar 2024
 *      Author: massimo
 */

#include "TCA.h"

TCA::TCA(void) {
	mName="";
	mForceMomentRef=NULL;
	mForceMomentHat=NULL;
	mTCAstate=NULL;
//	mMode=TCA_MANUAL;
//	mFunction="null";
//	mConfig="null";
}

TCA::~TCA(void) {
	if(mForceMomentRef!=NULL) {
		mForceMomentRef=NULL;
	}
	if(mForceMomentHat!=NULL) {
		mForceMomentHat=NULL;
	}
	if(mTCAstate!=NULL) {
		mTCAstate=NULL;
	}
}

void TCA::set_mode(string  str) {
	if((str==TCA_AUTO)||(str==TCA_MANUAL)||(str==TCA_RAW)) {
		mTCAstate->mMode=str;
	}
	else {
		printf("TCA::set_mode unexpected mode %s\n",str.c_str());
		fflush(stdout);
	}
}

void TCA::set_input_output(ForceMoment *forceMomentRef,ForceMoment *forceMomentHat) {
	mForceMomentRef=forceMomentRef;
	mForceMomentHat=forceMomentHat;
//	TCA::reset();
}


void TCA::execute(void) {

	//
	// compute applied force and torque according to actuator data
	//
	compute_applied_force_torque();

//	printf("BlueBoatTCA::SwampTCA mMode %s\n",mTCAstate->mMode.c_str());fflush(stdout);

	//
	// allocate thrust on actuators
	//
	if(mTCAstate->mMode==TCA_AUTO)
		execute_auto();
	else if(mTCAstate->mMode==TCA_MANUAL)
		execute_manual();
	else
		execute_raw();
//	printf_results();
}

