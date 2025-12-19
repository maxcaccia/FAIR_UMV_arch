/*
 * MipSimThread.h
 *
 *  Created on: Feb137, 2024
 *      Author: mc
 */

#ifndef MIPSIMTHREAD_H_
#define MIPPSIMTHREAD_H_

#include "CntrlSimThread.h"

#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "DevicePose.h"
#include "AhrsData.h"
//#include "MathAngles.h"
#include "AhrsSim.h"
#include "Matrix.h"
#include "nVector.h"
#include "FletcherChecksum.h"


class MipSimThread: public CntrlSimThread {
public:
	MipSimThread();
	virtual ~MipSimThread();

	virtual void make_thread(void);

	bool init_configuration(void); // function overwritten

protected:
	void execute(void) {CntrlSimThread::execute();}
//	void process_command(char *cmd);
	void process_data(char *data,ssize_t nRecv); // function to be overwritten
    void execute_thread_semantics(void);

private:
    void build_commands(void) {}
    void build_data(void);
    bool read_parameters(FILE *f);
    int build_packet(uint8_t* uint8ptr);
    void build_float(uint8_t* &uint8ptr,float value);
    void build_uint16(uint8_t* &uint8ptr,uint16_t value);

private:
    double dt;

    uint64_t mSamplingTime_ms;

    // state variables
    LinearPosition mMipPos;
    AngularPosition mMipAttitude;
	LinearVelocity mMipLinVel;
	AngularVelocity mMipAngVel;
//    Kinematics mMipState; // MIP kinematics
    // subscribed data
    LinearPosition mVehPos;
    AngularPosition mVehAttitude;
	LinearVelocity mVehLinVel;
	AngularVelocity mVehAngVel;
//    Kinematics mVehicleState; // rigid body kinematics
    // parameters: position and rotation with respect to the vehicle-fixed reference frame
    DevicePose mPose;

    // sensor measurements
    AhrsData mAhrsData;

    // sensors
    AhrsSim *mAhrsSim;

    double g;
};

#endif /* MIPSIMTHREAD_H_ */
