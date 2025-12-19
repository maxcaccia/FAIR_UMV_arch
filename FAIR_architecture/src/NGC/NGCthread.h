/*
 * NGCthread.h
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 *
 *  NGCthread is the parent class for <Robot>NGCthread
 */

#ifndef NGCTHREAD_H_
#define NGCTHREAD_H_

#include "CntrlSimThread.h"
#include "NGC.h"


class NGCthread: public CntrlSimThread {
public:
	NGCthread();
	virtual ~NGCthread();

	void make_thread(void);

	bool init_configuration(void);

protected:
    void process_command(char *cmd,ssize_t n); // { printf("NGCthread::process_command() %s\n",mName.c_str()); } // function to be overwritten
    void process_data(char *data,ssize_t nRecv); // { printf("NGCthread::process_data() %s %ld : %s\n",mName.c_str(),nRecv,data); } // function to be overwritten
    void execute_thread_semantics(void) { /*printf("NGCthread::execute_thread_semantics() %s\n",mName.c_str());*/ mNGC->execute();} // function to be overwritten

private:
    ////////////
    //
    // NGC data: references, estimates, measurements
    //
    //
    // Guidance, Autopilot & Control Tasks references
    //
    GuidanceReference mGuidanceRef;
    AutopilotReference mAutopilotRef;
    ControlReference mCntrlRef;
    //
    // robot kinematics estimates
    //
    LogicalPositionData mPositionHat;
    LogicalAttitudeData mAttitudeHat;
    LogicalSpeedData mSpeedHat;
    LogicalLinearRateData mLinearRateHat;
    LogicalLinearRateData mLinearRateWrtWaterHat;
    LogicalAngularRateData mAngularRateHat;
    //
    // sea current estimate
    //
    SeaCurrent mSeaCurrentHat;
	//
	// logical sensor measurements
	//
    LogicalPositionData mPositionData;
    LogicalAttitudeData mAttitudeData;
    LogicalSpeedData mSpeedData;
    LogicalLinearRateData mLinearRateData;
    LogicalAngularRateData mAngularRateData;

    ////////////
    //
    // TCA interface: robot force and torque
    //
    ForceMoment mForceMomentHat; // estimated force and moment
    ForceMoment mForceMomentRef; // reference force and moment

    ////////////
    //
    // NGC parameters
    //
    //
    // robot hydrodynamics
    //
    HydroDynamics mHydroPar;

    ////////////
    //
    // NGC: local NED frame latitude and longitude
    //
    double latitudeNED0;
    double longitudeNED0;

    ////////////
    //
    // NGC modules
    //
    NGC* mNGC;

};



#endif /* NGCTHREAD_H_ */
