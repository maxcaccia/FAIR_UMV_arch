/*
 * Navigation.cpp
 *
 *  Created on: 14 May 2024
 *      Author: massimo
 */

#include "Navigation.h"
#include "GuidanceControlAlgo.h"

Navigation::Navigation(void) {
	mName="";
    //
    // Force/Torque estimates
    //
    mForceMomentHat=NULL;
    //
    // logical sensor measurements
    //
    mPositionData=NULL;
    mAttitudeData=NULL;
    mSpeedData=NULL;
    mLinearRateData=NULL;
    mAngularRateData=NULL;

//    //
//    // sensor information (name and pose)
//    //
//    mForceTorqueInfo=NULL;
//    mGnssBodyInfo=NULL;
//    mGnssSternInfo=NULL;
//    mGnssBowInfo=NULL;
//    mAhrsBodyInfo=NULL;
    //
    // robot kinematics estimates
    //
    mPositionHat=NULL;
    mAttitudeHat=NULL;
    mSpeedHat=NULL;
    mLinearRateHat=NULL;
    mLinearRateWrtWaterHat=NULL;
    mAngularRateHat=NULL;

    //
    // robot hydrodynamics
    //
    mHydroPar=NULL;

    //
    // Motion Estimation
    //
    // attitude motion estimation
    mAttitudeLKf = NULL;

    mDt=NGC_DELTA_T;
    mTimeStamp=NULL;

}

void Navigation::reset(void) {
	//
	// reset estimators
	//
	mAttitudeLKf->reset();


//	mGnssEkf->reset();
}

Navigation::~Navigation(void) {
	if(mForceMomentHat!=NULL) {
		mForceMomentHat=NULL;
	}

	if(mPositionData!=NULL) {
		mPositionData=NULL;
	}
	if(mAttitudeData!=NULL) {
		mAttitudeData=NULL;
	}
	if(mSpeedData!=NULL) {
		mSpeedData=NULL;
	}
	if(mLinearRateData!=NULL) {
		mLinearRateData=NULL;
	}
	if(mAngularRateData!=NULL) {
		mAngularRateData=NULL;
	}

	if(mPositionHat!=NULL) {
		mPositionHat=NULL;
	}
	if(mAttitudeHat!=NULL) {
		mAttitudeHat=NULL;
	}
	if(mSpeedHat!=NULL) {
		mSpeedHat=NULL;
	}
	if(mLinearRateHat!=NULL) {
		mLinearRateHat=NULL;
	}
	if(mSeaCurrentHat!=NULL) {
		mSeaCurrentHat=NULL;
	}

	if(mAngularRateHat!=NULL) {
		mAngularRateHat=NULL;
	}


	if(mHydroPar!=NULL) {
		mHydroPar=NULL;
	}
}


//bool Navigation::instantiate_algorithms(void) {
//    // raw yaw motion estimation
//	mAttHeadEstimator = new AttitudeHeadingEstimator();
//    // surge-sway rate predictor (with respect to the water)
//    mSurgeSwayRatePredictor = new SurgeSwayRatePredictor(GC_DELTA_T);
//    // GNSS-based position and sea current estimator
//    mGnssEkf = new GnssEkf(GC_DELTA_T);
//
//    return true;
//}


void Navigation::set_control_actions(ForceMoment *forceMomentHat) {
	mForceMomentHat = forceMomentHat;
}


void Navigation::set_measurements(LogicalPositionData* positionData,LogicalAttitudeData* attitudeData,
		LogicalSpeedData* speedData,LogicalLinearRateData* linearRateData,
		LogicalAngularRateData* angularRateData) {
	mPositionData = positionData;
	mAttitudeData = attitudeData;
	mSpeedData = speedData;
	mLinearRateData = linearRateData;
	mAngularRateData = angularRateData;
}



void Navigation::set_estimates(LogicalPositionData* positionHat,LogicalAttitudeData* attitudeHat,
		LogicalSpeedData* speedHat,LogicalLinearRateData* linearRateHat,
		LogicalLinearRateData *linearRateWrtWaterHat,
		LogicalAngularRateData* angularRateHat,SeaCurrent *seaCurrentHat) {
	mPositionHat=positionHat;
	mAttitudeHat=attitudeHat;
	mSpeedHat=speedHat;
	mLinearRateHat=linearRateHat;
	mLinearRateWrtWaterHat=linearRateWrtWaterHat;
	mAngularRateHat=angularRateHat;
	mSeaCurrentHat=seaCurrentHat;
}


void Navigation::execute(void) {
	printf("Navigation::execute()\n"); fflush(stdout);
	//
	// execute Navigation
	//
	if(mAttitudeLKf!=NULL)
		mAttitudeLKf->execute();
	if(mHorLinEKf!=NULL)
		mHorLinEKf->execute();

}


void Navigation::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;

	// force & moment
	if(dataStrType==FORCE_MOMENT_CODE) {
		ForceMoment forceMomentData;
		forceMomentData.sread(data);
		*mForceMomentHat = forceMomentData;
		mForceMomentHat->mTimeStamp = *mTimeStamp;
	}
	// position
	else if(dataStrType==LOGICAL_POSITION_CODE) {
		LogicalPositionData positionData;
		positionData.sread(data);
		*mPositionData = positionData;
		mPositionData->mTimeStamp = *mTimeStamp;
	}
	// attitude
	else if(dataStrType==LOGICAL_ATTITUDE_CODE) {
		LogicalAttitudeData attitudeData;
		attitudeData.sread(data);
		*mAttitudeData = attitudeData;
		mAttitudeData->mTimeStamp = *mTimeStamp;
	}
	// speed
	else if(dataStrType==LOGICAL_SPEED_CODE) {
		LogicalSpeedData speedData;
		speedData.sread(data);
		*mSpeedData = speedData;
		mSpeedData->mTimeStamp = *mTimeStamp;
	}
	// linear rate
	else if(dataStrType==LOGICAL_LINEAR_RATE_CODE) {
		LogicalLinearRateData linearRateData;
		linearRateData.sread(data);
		*mLinearRateData = linearRateData;
		mLinearRateData->mTimeStamp = *mTimeStamp;
	}
	// angular rate
	else if(dataStrType==LOGICAL_ANGULAR_RATE_CODE) {
		LogicalAngularRateData angularRateData;
		angularRateData.sread(data);
		*mAngularRateData = angularRateData;
		mAngularRateData->mTimeStamp = *mTimeStamp;
	}
	// not processed
	else {
		printf("Navigation::process_data unexpected data type %s\n",dataStrType.c_str());
		fflush(stdout);
	}

}



void Navigation::print(void) {
	printf("===================================\n");
	printf("===   Navigation\n");
	printf("===================================\n");
	fflush(stdout);
	//
	// print Navigation algos
	//
	mAttitudeLKf->print();
}


bool Navigation::instantiate_tasks(void) {
	string name;
	string filename;
	//
	// instantiate attitude motion estimator
	//
	mAttitudeLKf = new AttitudeLKf2ndOrder180(mDt);
	name="attitude_lkf";
	mAttitudeLKf->set_name(name);
	mAttitudeLKf->set_input_output(mAttitudeData,mAngularRateHat,mAttitudeHat,mAngularRateHat);

	string s="Navigation/";
	filename=NGC_CONFIG_PATH+s+name+".cfg";
	if(!mAttitudeLKf->read_configuration(filename)) {
		printf("Navigation::instantiate_task error reading configuration file %s\n",filename.c_str());fflush(stdout);
		return false;
	}
	mAttitudeLKf->set_time_stamp(mTimeStamp);
	mAttitudeLKf->reset();

	//
	// instantiate horizontal linear motion estimator
	//
	mHorLinEKf = new MultirateEKFhorLin(mDt);
	name="hor_lin_ekf";
	mHorLinEKf->set_name(name);
	mHorLinEKf->set_input_output(mPositionData,mSpeedData,mLinearRateData,
			mAttitudeHat,mAngularRateHat,
			mPositionHat,mLinearRateWrtWaterHat,mSeaCurrentHat);
	filename=NGC_CONFIG_PATH+s+name+".cfg";
	if(!mHorLinEKf->read_configuration(filename)) {
		printf("Navigation::instantiate_task error reading configuration file %s\n",filename.c_str());fflush(stdout);
		return false;
	}
	mHorLinEKf->set_time_stamp(mTimeStamp);
	mHorLinEKf->reset();

	return true;
}

