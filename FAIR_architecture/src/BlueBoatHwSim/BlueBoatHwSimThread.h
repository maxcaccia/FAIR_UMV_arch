/*
 * BlueBoatHwSimThread.h
 *
 *  Created on: Mar 26, 2025
 *      Author: mc
 */

#ifndef BLUEBOATHWSIMTHREAD_H_
#define BLUEBOATHWSIMTHREAD_H_

#include "CntrlSimThread.h"

#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "DevicePose.h"
//#include "ForceTorque.h"
#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "AhrsData.h"
#include "NormalisedRpm.h"
#include "MathAngles.h"
#include "GnssSim.h"
#include "AhrsSim.h"
#include "Matrix.h"
#include "nVector.h"

#include "Commands.h"

#include "../../mavlink/common/mavlink.h"

#define BATTERY_VOLTAGE_FULL 16.0

#define THROTTLE_LEFT	73
#define THROTTLE_RIGHT	74
#define DISABLED		0

class BlueBoatHwSimThread: public CntrlSimThread {
public:
	BlueBoatHwSimThread();
	virtual ~BlueBoatHwSimThread();

	virtual void make_thread(void);

	bool init_configuration(void); // function overwritten

protected:
	void execute(void) {CntrlSimThread::execute();}
	void process_command(char *cmd,ssize_t n);
	void process_data(char *data,ssize_t nRecv); // function to be overwritten
    void execute_thread_semantics(void);

private:
    void build_commands(void);
    void build_data(void);
    void build_force_torque_cmd(void);
    bool read_parameters(FILE *f);

private:
    int mId;
    double dt;

    uint64_t mSamplingTime_ms;

    // state variables
    LinearPosition mBlueBoatPos;
    AngularPosition mBlueBoatAttitude;
	LinearVelocity mBlueBoatLinVel;
	AngularVelocity mBlueBoatAngVel;
    // subscribed data
    LinearPosition mVehPos;
    AngularPosition mVehAttitude;
	LinearVelocity mVehLinVel;
	AngularVelocity mVehAngVel;
    // parameters: position and rotation with respect to the vehicle-fixed reference frame
    DevicePose mPose;

    // sensor measurements
    GnssPositionData mGnssPosData;
    GnssVelocityData mGnssVelData;
    AhrsData mAhrsData;
    // actuator data
	NormalisedRpm mNormRpm; // normaised left and right rpm

    // commands to vehicle
//    ForceTorque mForceTorque;
	RigidBodyCmd mRbCmd;

    // state variables: mode and arm
    string mMode;
    string mArm;
    double mBatteryVoltage;

    double mServo1Function;
    double mServo3Function;

    // sensors
    GnssSim *mGnssSim;
    AhrsSim *mAhrsSim;

    // force and torque applied to the vehicle
	Vector F,T;

	// propeller rpm
	double mLeftRpm;
	double mRightRpm;

	bool mHeartBeatFlag;
};



#endif /* BLUEBOATHWSIMTHREAD_H_ */
