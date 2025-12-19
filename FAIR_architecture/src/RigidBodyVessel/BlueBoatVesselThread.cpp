/*
 * BlueBoatVesselThread.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: mc
 */

#include "BlueBoatVesselThread.h"

BlueBoatVesselThread::BlueBoatVesselThread(void): RigidBodyVesselThread() {
//	printf("BlueBoatVesselThread::BlueBoatVesselThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mActuatorNumber=1;
}

BlueBoatVesselThread::~BlueBoatVesselThread() {
	// TODO Auto-generated destructor stub

}

void BlueBoatVesselThread::make_thread()
{
//    printf("BlueBoatVesselThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&BlueBoatVesselThread::execute,this);
}



void BlueBoatVesselThread::dynamics(void) {
	double ur=mLinearSpeed.ur;
	double vr=mLinearSpeed.vr;
	double r=mAngSpeed.r*(M_PI/180.);
    //
    // compute total force and torque
    //
	double fu = mPar.Xu*ur + mPar.Xuu*ur*fabs(ur) + mPar.mv*vr*r + mPar.kr2*SQR(r) + (1. - mPar.Xu * ur) * mForceMoment.X;
	double fv = mPar.Yv*vr + mPar.Yvv*vr*fabs(vr) - mPar.mu*ur*r + mForceMoment.Y;
	double Nr = mPar.Nr*r + mPar.Nrr*r*fabs(r) - mPar.kur*ur*r + (mPar.mu-mPar.mv)*ur*vr + mForceMoment.N;
    //
    // solve speed equation with respect to the water
    //
	double uDot=fu/mPar.mu;
	//
	// solve steering equation with respect to the water
	//
	Matrix M(2,2);
	M(0,0)=mPar.mv;
	M(1,1)=mPar.Ir;
	M(0,1)=M(1,0)=mPar.kr2;
	Matrix x(2,1),b(2,1);
	b(0,0)=fv;
	b(1,0)=Nr;
	x=M.inv()*b;
	double vDot=x(0,0);
	double rDot=x(1,0);
	//
    // compute linear and angular speed with respect to the water
    //
	mLinearSpeed.ur+=uDot*dt;
	mLinearSpeed.vr+=vDot*dt;
	mAngSpeed.r+=(rDot*dt)*180./M_PI;

//    printf("RigidBodyVesselThread::dynamics(void) dt %lf N %lf Nr %lf r %lf\n",dt,mForceTorque.N,Nr,r);fflush(stdout);
}



void BlueBoatVesselThread::process_force_moment_command() {
//	printf("******************** %s ---- BlueBoatVesselThread::process_force_moment_command : %s\r\n",mName.c_str(),mRigidBodyCmd.actuatorId.c_str());fflush(stdout);

	printf(" %s ---- BlueBoatVesselThread::process_force_moment_command : %.2lf %.2lf\r\n",mName.c_str(),mRigidBodyCmd.X,mRigidBodyCmd.N);fflush(stdout);
	//
	// ideal force-torque vector applied to the rigid body
	//
	get_force_moment(mActuatorForceMoment[0],mRigidBodyCmd);


//	if(mRigidBodyCmd.actuatorId=="ideal-actuator") {
//		//
//		// ideal force-torque vector applied to the rigid body
//		//
//		get_force_moment(mActuatorForceMoment[0],mRigidBodyCmd);
//	}
//	else {
//		printf("BlueBoatVesselThread::process_force_moment_command ERROR received SET_FORCE_MOMENT with actuatorId %s\n",mRigidBodyCmd.actuatorId.c_str());
//		fflush(stdout);
//	}
}





