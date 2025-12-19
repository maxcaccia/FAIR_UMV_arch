/*
 * NGC.h
 *
 *  Created on:17 May 2024
 *      Author: massimo
 */

#ifndef NGC_H_
#define NGC_H_

#include <stdio.h>

//#include <inttypes.h>
#include "ExecutionControl.h"
#include "Navigation.h"
#include "Guidance.h"
#include "Autopilot.h"
#include "Control.h"
#include "HydroDynamics.h"


class NGC {
protected:
	string mName;
	string mConfigDir;
    ////////////
    //
    // NGC data: references, estimates, measurements, parameters
    //
    //
    // Guidance, Autopilot & Control Tasks references
    //
    GuidanceReference* mGuidanceRef;
    AutopilotReference* mAutopilotRef;
    ControlReference* mCntrlRef;
    //
    // robot kinematics estimates
    //
    LogicalPositionData* mPositionHat;
    LogicalAttitudeData* mAttitudeHat;
    LogicalSpeedData* mSpeedHat;
    LogicalLinearRateData* mLinearRateHat;
    LogicalLinearRateData *mLinearRateWrtWaterHat;
    LogicalAngularRateData* mAngularRateHat;
    //
    // sea current estimate
    //
    SeaCurrent *mSeaCurrentHat;

    //
    // logical sensor measurements
    //
    LogicalPositionData* mPositionData;
    LogicalAttitudeData* mAttitudeData;
    LogicalSpeedData* mSpeedData;
    LogicalLinearRateData* mLinearRateData;
    LogicalAngularRateData* mAngularRateData;

    //
    // TCA interface: robot force and torque
    //
    ForceMoment* mForceMomentHat; // estimated force and moment
    ForceMoment* mForceMomentRef; // reference force and moment
    //
    // robot hydrodynamics
    //
    HydroDynamics* mHydroPar;

    double* mTimeStamp;

    ////////////
    //
    // NGC modules: guidance, autopilot, control, navigation
    //
    ExecutionControl mExecutionControl;
    Guidance mGuidance;
    Autopilot mAutopilot;
    Control mControl;
    Navigation mNavigation;

    ////////////
    //
    // NGC: local NED frame latitude and longitude
    //
    double *mLatitudeNED0;
    double *mLongitudeNED0;

public:
	// constructor/destructor
	NGC();
    ~NGC(void);
    void reset(void);
    void set_name(string name) { mName=name; }
    void set_config_dir(string configDir) { mConfigDir=configDir; }
    // configure
    void set_gac_references(GuidanceReference* guidanceRef,AutopilotReference* autopilotRef,ControlReference* cntrlRef)
    	{ mGuidanceRef=guidanceRef; mAutopilotRef=autopilotRef; mCntrlRef=cntrlRef; }

    void set_estimates(LogicalPositionData* positionHat,LogicalAttitudeData* attitudeHat,
    		LogicalSpeedData* speedHat,LogicalLinearRateData* linearRateHat,
			LogicalLinearRateData *linearRateWrtWaterHat,
			LogicalAngularRateData* angularRateHat,SeaCurrent *seaCurrentHat)
    	{ mPositionHat=positionHat; mAttitudeHat=attitudeHat; mSpeedHat=speedHat;
    	  mLinearRateHat=linearRateHat; mLinearRateWrtWaterHat=linearRateWrtWaterHat;
    	  mAngularRateHat=angularRateHat; mSeaCurrentHat=seaCurrentHat;}

    void set_logical_measurements(LogicalPositionData* positionData,LogicalAttitudeData* attitudeData,
    		LogicalSpeedData* speedData,LogicalLinearRateData* linearRateData,
			LogicalAngularRateData* angularRateData)
    	{ mPositionData=positionData; mAttitudeData=attitudeData;
    	mSpeedData=speedData; mLinearRateData=linearRateData;
    	mAngularRateData=angularRateData; }

    void set_TCA_interface(ForceMoment* forceMomentHat,ForceMoment* forceMomentRef)
    	{ mForceMomentHat=forceMomentHat; mForceMomentRef=forceMomentRef; }
    void set_hydrodynamics(HydroDynamics* hydroPar)
    	{ mHydroPar=hydroPar; }
    void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
    void set_local_NED_frame(double *latitudeNED0,double *longitudeNED0)
    	{ mLatitudeNED0=latitudeNED0; mLongitudeNED0=longitudeNED0; }

    bool configure_modules(void);
    bool fread_local_NED_frame(string filename);

    void process_command(char *cmd);
    void process_data(char *data,ssize_t nRecv);
   // execute
	void execute(void); // {if(mMode==TCA_AUTO) execute_auto(); else if(mMode==TCA_MANUAL) execute_manual(); else execute_raw(); printf_results();}

	void print(void);

	void printf_results(void)  {printf("void NGC::printf_results  %s\n",mName.c_str());fflush(stdout);}
};



#endif /* NGC_H_ */
