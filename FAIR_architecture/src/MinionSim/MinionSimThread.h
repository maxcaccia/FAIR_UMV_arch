/*
 * MinionSimThread.h
 *
 *  Created on: Jan 12, 2024
 *      Author: mc
 */

#ifndef MINIONSIMTHREAD_H_
#define MINIONSIMTHREAD_H_

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
#include "SpeedMotorData.h"
#include "PositionMotorData.h"
#include "MathAngles.h"
#include "GnssSim.h"
#include "AhrsSim.h"
#include "SpeedMotorSim.h"
#include "PositionMotorSim.h"
#include "MinionDriverInterface.h"
#include "Matrix.h"
#include "nVector.h"

#include "Commands.h"

//#define MAX_PUMP_MOTOR_SPEED 3240 // 1800
//#define START_BATTERY_VOLTAGE 29.0
#define BATTERY_VOLTAGE_FULL 28.6

//#define THRUST_RPM_MAX 3240.
//#define THRUST_SPEED_PERC_FACTOR (THRUST_RPM_MAX/4.0)

class MinionSimThread: public CntrlSimThread {
public:
	MinionSimThread();
	virtual ~MinionSimThread();

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
    void build_force_torque_cmd(double thrust,double azimuthDeg);
    bool read_parameters(FILE *f);

private:
    int mId;
    double dt;

    uint64_t mSamplingTime_ms;

    // state variables
    LinearPosition mMinionPos;
    AngularPosition mMinionAttitude;
	LinearVelocity mMinionLinVel;
	AngularVelocity mMinionAngVel;
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
	SpeedMotorData mSpeedMotorData; // pump-jet motor
	PositionMotorData mPositionMotorData; // azimuth motor

    // commands to vehicle
//    ForceTorque mForceTorque;
	RigidBodyCmd mRbCmd;

    // state variables: digital I/O and battery
    uint16_t mDigitalInput;
    uint16_t mDigitalOutput;
    double mBatteryVoltage;

    // sensors
    GnssSim *mGnssSim;
    AhrsSim *mAhrsSim;
    // actuators
    SpeedMotorSim *mSpeedMotorSim;
    PositionMotorSim *mPositionMotorSim;

    // force and torque applied to the vehicle
	Vector F,T;
};



#endif /* MINIONSIMTHREAD_H_ */
