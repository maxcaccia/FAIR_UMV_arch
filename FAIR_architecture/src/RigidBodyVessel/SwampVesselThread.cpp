/*
 * SwampVesselThread.cpp
 *
 *  Created on: Jan 12, 2024
 *      Author: mc
 */

#include "SwampVesselThread.h"

SwampVesselThread::SwampVesselThread(void): RigidBodyVesselThread() {
//	printf("SwampVesselThread::SwampVesselThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mActuatorNumber=4;
}

SwampVesselThread::~SwampVesselThread() {
	// TODO Auto-generated destructor stub

}

void SwampVesselThread::make_thread()
{
//    printf("SwampVesselThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&SwampVesselThread::execute,this);
}


void SwampVesselThread::process_force_moment_command() {
//	printf("******************** %s ---- SwampVesselThread::process_force_moment_command : %s\r\n",mName.c_str(),mRigidBodyCmd.actuatorId.c_str());fflush(stdout);

	if(mRigidBodyCmd.actuatorId=="minionRL-sim")
		get_force_moment(mActuatorForceMoment[0],mRigidBodyCmd);
	else if(mRigidBodyCmd.actuatorId=="minionFL-sim")
		get_force_moment(mActuatorForceMoment[1],mRigidBodyCmd);
	else if(mRigidBodyCmd.actuatorId=="minionRR-sim")
		get_force_moment(mActuatorForceMoment[2],mRigidBodyCmd);
	else if(mRigidBodyCmd.actuatorId=="minionFR-sim")
		get_force_moment(mActuatorForceMoment[3],mRigidBodyCmd);
	else if(mRigidBodyCmd.actuatorId=="ideal-actuator") {
		//
		// ideal force-torque vector applied to the rigid body
		//
		get_force_moment(mActuatorForceMoment[0],mRigidBodyCmd);
		mActuatorForceMoment[1].reset();
		mActuatorForceMoment[2].reset();
		mActuatorForceMoment[3].reset();
	}
	else {
		printf("SwampVesselThread::process_force_moment_command ERROR received SET_FORCE_MOMENT with actuatorId %s\n",mRigidBodyCmd.actuatorId.c_str());
		fflush(stdout);
	}
}





