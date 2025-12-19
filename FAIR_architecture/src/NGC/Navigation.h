/*
 * Navigation.h
 *
 *  Created on: 17 July 2024
 *      Author: massimo
 */

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <stdio.h>

#include <inttypes.h>
#include "NGCconfig.h"
#include "ForceMoment.h"
#include "LogicalAttitudeData.h"
#include "LogicalAngularRateData.h"
#include "LogicalPositionData.h"
#include "LogicalLinearRateData.h"
#include "LogicalSpeedData.h"
#include "SeaCurrent.h"
#include "HydroDynamics.h"
#include "AttitudeLKf2ndOrder180.h"
#include "MultirateEKFhorLin.h"

//#include "AttitudeHeadingEstimator.h"
//#include "LinearVelocityPredictor.h"
//#include "GnssEkf.h"

class Navigation {
private:
	string mName;
    //
    // Force/Torque estimates
    //
    ForceMoment *mForceMomentHat;
    //
    // logical sensor measurements
    //
    LogicalPositionData *mPositionData;
    LogicalAttitudeData *mAttitudeData;
    LogicalSpeedData *mSpeedData;
    LogicalLinearRateData *mLinearRateData;
    LogicalAngularRateData *mAngularRateData;

    //
    // robot kinematics estimates
    //
    LogicalPositionData *mPositionHat;
    LogicalAttitudeData *mAttitudeHat;
    LogicalSpeedData *mSpeedHat;
    LogicalLinearRateData *mLinearRateHat;
    LogicalLinearRateData *mLinearRateWrtWaterHat;
    LogicalAngularRateData *mAngularRateHat;
    //
    // sea current estimate
    //
    SeaCurrent *mSeaCurrentHat;

    //
    // robot hydrodynamics
    //
    HydroDynamics* mHydroPar;

    //
    // Motion Estimation
    //
    // attitude motion estimation
    AttitudeLKf2ndOrder180* mAttitudeLKf;
    // linear horizontal motion estimation
    MultirateEKFhorLin* mHorLinEKf;

    // GNSS-based linear position, speed and sea current estimator
//    EKF2ndOrderXY* mLinearMotionEkf;

    double mDt;
    double* mTimeStamp;

public:
	// constructor/destructor
	Navigation();
    virtual ~Navigation(void);

    void set_name(string name) { mName=name; }
//    // configure
//    bool instantiate_algorithms(void);
    // process commands
    void process_command(char *cmd);
    // set control actions (force and moment)
    void set_control_actions(ForceMoment *forceMomentHat);
    // set logical measurements
    void set_measurements(LogicalPositionData* positionData,LogicalAttitudeData* attitudeData,
    		LogicalSpeedData* speedData,LogicalLinearRateData* linearRateData,
			LogicalAngularRateData* angularRateData);
    // set estimates
    void set_estimates(LogicalPositionData* positionHat,LogicalAttitudeData* attitudeHat,
    		LogicalSpeedData* speedHat,LogicalLinearRateData* linearRateHat,
			LogicalLinearRateData *linearRateWrtWaterHat,
			LogicalAngularRateData* angularRateHat,SeaCurrent *seaCurrentHat);
    // set hydrodynamics model
    void set_hydrodynamics(HydroDynamics *hydroPar) { mHydroPar=hydroPar; }

    void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
    // process data
    void process_data(char *data,ssize_t nRecv);
    // execute
	void execute(void);
	void reset(void);
	// configure and instantiate
	bool instantiate_tasks(void);
//	bool read_sensor_allocation(string filename);

	void print(void);

protected:
	virtual void printf_results(void)  {printf("virtual void Navigation::printf_results  %s\n",mName.c_str());fflush(stdout);}
};



#endif /* NAVIGATION_H_ */
