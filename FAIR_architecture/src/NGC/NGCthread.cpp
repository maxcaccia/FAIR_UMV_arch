/*
 * NGCthread.cpp
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#include "NGCthread.h"

NGCthread::NGCthread(void): CntrlSimThread() {

//	printf("NGCthread::NGCthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mNGC = new NGC();

    latitudeNED0 = 0.0;
    longitudeNED0 = 0.0;

}

NGCthread::~NGCthread() {
	// TODO Auto-generated destructor stub
}

void NGCthread::make_thread()
{
//    printf("NGCthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&NGCthread::execute,this);
}

bool NGCthread::init_configuration(void) {
//	printf("NGCthread::init_configuration configDir %s\n",mThreadProperties.configDir.c_str());

	string configDir = mThreadProperties.configDir+"ngc/";
	mNGC->set_config_dir(configDir);
//	mNGC->set_config_dir(mThreadProperties.configDir);

	mNGC->set_gac_references(&mGuidanceRef, &mAutopilotRef, &mCntrlRef);
	mNGC->set_estimates(&mPositionHat, &mAttitudeHat, &mSpeedHat, &mLinearRateHat,
			&mLinearRateWrtWaterHat, &mAngularRateHat, &mSeaCurrentHat);

	mNGC->set_logical_measurements(&mPositionData, &mAttitudeData, &mSpeedData,
			&mLinearRateData,&mAngularRateData);

	mNGC->set_TCA_interface(&mForceMomentHat, &mForceMomentRef);
	mNGC->set_hydrodynamics(&mHydroPar);
	mNGC->set_time_stamp(&mTimeStamp);

	mNGC->set_local_NED_frame(&latitudeNED0, &longitudeNED0);

	if(!mNGC->configure_modules()) {
		printf("NGCthread::init_configuration : ERROR mNGC->configure_modules() failed!\n");
		fflush(stdout);
		return false;
	}

	return true;
}

void NGCthread::process_command(char *cmd,ssize_t n) {
	mNGC->process_command(cmd);
}


void NGCthread::process_data(char *data,ssize_t nRecv) {
	mNGC->process_data(data,nRecv);
}


