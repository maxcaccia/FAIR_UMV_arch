/*
 * GnssSimThread.h
 *
 *  Created on: Feb 7, 2024
 *      Author: mc
 */

#ifndef GNSSSIMTHREAD_H_
#define GNSSSIMTHREAD_H_

#include "CntrlSimThread.h"

#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "DevicePose.h"
#include "GnssPositionData.h"
#include "GnssVelocityData.h"
//#include "MathAngles.h"
#include "GnssSim.h"
#include "Matrix.h"
#include "nVector.h"


class GnssSimThread: public CntrlSimThread {
public:
	GnssSimThread();
	virtual ~GnssSimThread();

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

private:
    double dt;

    uint64_t mSamplingTime_ms;

    // state variables
    LinearPosition mGnssPos;
    AngularPosition mGnssAttitude;
	LinearVelocity mGnssLinVel;
	AngularVelocity mGnssAngVel;
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

    // sensors
    GnssSim *mGnssSim;
};



#endif /* GNSSSIMTHREAD_H_ */
