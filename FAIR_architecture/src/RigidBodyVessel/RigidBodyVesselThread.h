/*
 * RigidBodyVesselThread.h
 *
 *  Created on: Jan 12, 2024
 *      Author: mc
 */

#ifndef RIGIDBODYVESSELTHREAD_H_
#define RIGIDBODYVESSELTHREAD_H_

#include "CntrlSimThread.h"

#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "ForceMoment.h"
#include "HydroDynamics.h"

#include "Commands.h"

#define Nmax_ACTUATORS 4

class RigidBodyVesselThread: public CntrlSimThread {
public:
	RigidBodyVesselThread();
	virtual ~RigidBodyVesselThread();

	virtual void make_thread(void);

	bool init_configuration(void); // function overwritten

protected:
	void execute(void) {CntrlSimThread::execute();}
    virtual void process_force_moment_command() {printf("******************** %s ---- RigidBodyVesselThread::process_force_moment_command : %s\r\n",mName.c_str(),mRigidBodyCmd.actuatorId.c_str());fflush(stdout);} // function to be overwritten
//    void process_data(char *data,ssize_t nRecv); // function to be overwritten
    void execute_thread_semantics(void);

private:
    void process_command(char *cmd,ssize_t n);
    void compute_force_torque(void);
    virtual void dynamics(void);
    void kinematics(void);

    void build_commands(void) {}
    void build_data(void);

protected:
    double dt;
//    Kinematics mState; // rigid body kinematics
    LinearPosition mPosition;
    AngularPosition mAttitude;
    LinearVelocity mLinearSpeed;
    AngularVelocity mAngSpeed;
    ForceMoment mForceMoment;
    HydroDynamics mPar; // rigid body hydrodynamic parameters

protected:
    RigidBodyCmd mRigidBodyCmd;

protected:
    int mActuatorNumber;
    ForceMoment mActuatorForceMoment[Nmax_ACTUATORS];
};

void get_force_moment(ForceMoment &tau,RigidBodyCmd cmd);

#endif /* RIGIDBODYVESSELTHREAD_H_ */
