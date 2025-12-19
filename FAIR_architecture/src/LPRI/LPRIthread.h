/*
 * LPRIthread.h
 *
 *  Created on: Jul 11, 2025
 *      Author: mc
 *
 *  LPRIthread is the parent class for <Robot>LPRIthread
 */

#ifndef LPRITHREAD_H_
#define LPRITHREAD_H_

#include "CntrlSimThread.h"
#include "XYN.h"
#include "XYNnorm.h"
#include "TCAstate.h"
#include "TCA.h"
#include "Commands.h"
#include "P2LSM.h"

class LPRIthread: public CntrlSimThread {
public:
	LPRIthread();
	virtual ~LPRIthread();

	virtual void make_thread(void);

	virtual bool init_configuration(void);

protected:
    void process_command(char *cmd,ssize_t n);
    void process_data(char *data,ssize_t nRecv);
	//    virtual void process_data(char *data,ssize_t nRecv) { printf("TCAthread::process_data() %s %ld\n",mName.c_str(),nRecv); } // function to be overwritten
    virtual void execute_thread_semantics(void) { mTCAstate.mTimeStamp = mTimeStamp; /*mTCA->execute(); mP2LSM->execute();*/ }

    virtual void build_commands(void) {printf("virtual void LPRIthread::build_commands\n");fflush(stdout);}
    virtual void build_data(void);

protected:
    ////////////
    //
    // Thrust Control Allocation - TCA
    //
    ////////////
    //
    // TCA data: references
    //
    //
    // robot force and torque
    //
    XYN mXYNref; // reference force and moment
    XYN mXYNhat; // estimated force and moment
    XYNnorm mXYNnormRef; // reference normalised force and moment
    XYNnorm mXYNnormHat; // estimated normalised force and moment
    //
    // robot actuation : to be declared in the child class <Robot>TCAthread
    //
    // <Robot>Actuation m<Robot>ActRef; // reference robot actuation
    // <Robot>Actuation m<Robot>ActMeas; // measured robot actuation
    //
    // state
    TCAstate mTCAstate;
    ////////////
    //
    // TCA modules
    //
    TCA* mTCA;

    ////////////
    //
    // Physical to Logical Sensor Mapping - P2LSM
    //
    ////////////
    //
    // P2LSM state : active logical sensors
    //
    P2LSMstate mP2LSMstate;

    ////////////
    //
    // sensor information (name, pose, measurement uncertainty)
    //
    vector<SensorInfo> mSensorInfo;
//    SensorInfo mSensorInfo[MAX_N_SENSORS];
//    int nSensor;

    ////////////
    //
    // Driver Interface - sensor data: AHRS, GNSS-GGA, GNSS-VTG, DVL
    //
    AhrsData mAhrsData;
    GnssPositionData mGnssPositionData;
    GnssVelocityData mGnssVelocityData;
    DvlData mDvlData;

    ////////////
    //
    // Logical Sensors: Physical Sensor priorities
    //
    vector<string> mLogicalPosition;
    vector<string> mLogicalAttitude;
    vector<string> mLogicalSpeed;
    vector<string> mLogicalLinearRate;
    vector<string> mLogicalAngularRate;

    ////////////
    //
    // NGC interface - Logical data
    //
    LogicalPositionData mLogicalPositionData;
    LogicalAttitudeData mLogicalAttitudeData;
    LogicalSpeedData mLogicalSpeedData;
    LogicalLinearRateData mLogicalLinearRateData;
    LogicalAngularRateData mLogicalAngularRateData;

    ////////////
    //
    // P2LSM module
    //
    P2LSM* mP2LSM;


protected:
    ////////////
//    //
//    // TCA command
//    //
//    TcaCmd mTcaCmd;

};



#endif /* LPRITHREAD_H_ */
