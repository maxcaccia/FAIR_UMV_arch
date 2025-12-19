/*
 * RigidBodyVesselThread.cpp
 *
 *  Created on: Jan 12, 2024
 *      Author: mc
 */

#include "RigidBodyVesselThread.h"
#include "MathAngles.h"

RigidBodyVesselThread::RigidBodyVesselThread(void): CntrlSimThread() {
//	printf("RigidBodyVesselThread::RigidBodyVesselThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mActuatorNumber=0;
	dt=0.;
}

RigidBodyVesselThread::~RigidBodyVesselThread() {
	// TODO Auto-generated destructor stub

}

void RigidBodyVesselThread::make_thread()
{
//    printf("RigidBodyVesselThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&RigidBodyVesselThread::execute,this);
}

bool RigidBodyVesselThread::init_configuration(void) {
	//
	// initialise dt, i.e. integration interval
	//
	dt=(double)mThreadProperties.timeValue/1000.;
	//
	// initialise rigid body simulator
	//
//	FILE *f;
	string fileName;
//	//
//	// read number of actuators
//	//
//	fileName=mThreadProperties.configDir+mName+"-Actuation.cfg";
//	if((f=fopen(fileName.c_str(),"r"))==NULL) {
//		printf("%s RigidBodyVesselThread::read_configuration ERROR : cannot open %s\n",mName.c_str(),fileName.c_str());
//		fflush(stdout);
//		return false;
//	}
//	char str[64];
//	fscanf(f,"%s %d",str,&mActuatorNumber);
//    if(strcmp(str,"nActuators")!=0)
//    {
//        printf("RigidBodyVesselThread::read_configuration nActuators expected %s found!\n",str);fflush(stdout);
//        fclose(f);
//        return false;
//    }
    //
    // read rigid body hydrodynamics
    //
    mPar.set_name(mName);
    fileName=mThreadProperties.configDir+"HIL-sim/"+mName+"-HydroDynamics.cfg";
	if(!mPar.fread(fileName)) {
		printf("RigidBodyVesselThread::read_configuration failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}
    //
    // read rigid body kinematics
    //
	mPosition.set_name(mName);
	mAttitude.set_name(mName);
	mLinearSpeed.set_name(mName);
	mAngSpeed.set_name(mName);
	mLinearSpeed.set_attitude(&mAttitude);
    fileName=mThreadProperties.configDir+"HIL-sim/"+mName+"-Kinematics.cfg";
    FILE *f;
	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("RigidBodySim::read_configuration failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}
	if(!mPosition.fread(f))
		return false;
	if(!mAttitude.fread(f))
		return false;
	if(!mAngSpeed.fread(f))
		return false;
	if(!mLinearSpeed.fread(f))
		return false;
	//
	// set mForceTorque name
	//
	mForceMoment.set_name(mName);
	return true;
}


void RigidBodyVesselThread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- RigidBodyVesselThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	char chrType[64], chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="RIGID_BODY") {
		if(mRigidBodyCmd.decode(cmd, strCmd)) {
			if(strCmd=="SET_FORCE_MOMENT") {
				process_force_moment_command();
			}
			else  {
				printf("RigidBodyVesselThread::decode ERROR read - RIGID_BODY cmd is %s\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="RIGID_BODY")
	else  {
		printf("RigidBodyVesselThread::process_command ERROR read - type is %ss\n",strType.c_str());
		fflush(stdout);
		return ;
	}


//	char cmdType[64];
//	sscanf(cmd,"%s",cmdType);
//	string cmdStrType=cmdType;
//	if(cmdStrType=="FORCE_TORQUE")
//		process_force_torque_command(cmd);

}

//void RigidBodyVesselThread::process_data(char *data,ssize_t nRecv) {
//	char dataType[64];
//	sscanf(data,"%s",dataType);
//	string dataStrType=dataType;
//	if(dataStrType=="AHRS")
//		mAhrsData.sread(data);
//	else if(dataStrType=="GPS")
//		mGpsData.sread(data);
//	else if(dataStrType=="TRUE_HEADING")
//		mTrueHeadingData.sread(data);
//	else if(dataStrType=="KIN_DYN")
//		mKinDynHat.sread(data);
//	else if(dataStrType=="FORCE_TORQUE")
//		mForceTorqueRef.sread(data);
//	else if(dataStrType=="SWAMP_TLM")
//		mSwampTlm.sread(data);
//
//}

void RigidBodyVesselThread::compute_force_torque(void) {
	mForceMoment.reset();
	for(int i=0;i<mActuatorNumber;i++) {
		mForceMoment.X+=mActuatorForceMoment[i].X;
		mForceMoment.Y+=mActuatorForceMoment[i].Y;
		mForceMoment.Z+=mActuatorForceMoment[i].Z;
		mForceMoment.K+=mActuatorForceMoment[i].K;
		mForceMoment.M+=mActuatorForceMoment[i].M;
		mForceMoment.N+=mActuatorForceMoment[i].N;
	}
}

void RigidBodyVesselThread::dynamics(void) {
	double ur=mLinearSpeed.ur;
	double vr=mLinearSpeed.vr;
	double r=mAngSpeed.r*(M_PI/180.);
    //
    // compute total force and torque
    //
	double fu = mPar.Xu*ur + mPar.Xuu*ur*fabs(ur) + mPar.mv*vr*r + mPar.m*mPar.xg*SQR(r) + mForceMoment.X;
	double fv = mPar.Yv*vr + mPar.Yvv*vr*fabs(vr) - mPar.mu*ur*r + mForceMoment.Y;
	double Nr = mPar.Nr*r + mPar.Nrr*r*fabs(r) - mPar.m*mPar.xg*ur*r + (mPar.mu-mPar.mv)*ur*vr + mForceMoment.N;
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
	M(0,1)=M(1,0)=mPar.m*mPar.xg;
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

void RigidBodyVesselThread::kinematics(void) {
    //
	// compute sea current
	//
	// TO DO: get sea current from (x,y,time)
	//
	// compute heading
    mAttitude.psi=summod180(mAttitude.psi,mAngSpeed.r*dt);
    // compute linear velocity and speed (NED)
    mLinearSpeed.compute_NED_velocity_speed_from_body_relative_speed_and_sea_current(&mAttitude);
    // compute linear velocity in vehicle-fixed frame
    mLinearSpeed.compute_body_velocity_from_NED_velocity(&mAttitude);
    // compute latitude and longitude
    lat_lon_plus_x_y(mPosition.latitude,mPosition.longitude,mPosition.latitude,mPosition.longitude,
    		mLinearSpeed.xDot*dt,mLinearSpeed.yDot*dt);
    // compute x and y
    lat_lon_2_x_y(mPosition.x,mPosition.y,mPosition.lat0,mPosition.lon0,mPosition.latitude,mPosition.longitude);

}

void RigidBodyVesselThread::execute_thread_semantics(void) {
//	printf("RigidBodyVesselThread::execute_thread_semantics() %s %" PRIu64 "\n",mName.c_str(),mElapsedMilliseconds);fflush(stdout);
	compute_force_torque();
	dynamics();
	kinematics();
//	//
//	// set time stamp & valid flag
//	//
//    time_t t;
//    struct tm *tmp;
//    t = time(NULL);
//    tmp = gmtime(&t);
//    double time = tmp->tm_hour*10000.+tmp->tm_min*100.+tmp->tm_sec+(mElapsedMilliseconds%1000)/1000.; // hhmmss.ss in UTC

    mPosition.mTimeStamp = mTimeStamp;
    mAttitude.mTimeStamp = mTimeStamp;
    mLinearSpeed.mTimeStamp = mTimeStamp;
    mAngSpeed.mTimeStamp = mTimeStamp;
    mForceMoment.mTimeStamp = mTimeStamp;
//    mPosition.mTimeStamp = mElapsedMilliseconds;
//    mAttitude.mTimeStamp = mElapsedMilliseconds;
//    mLinearSpeed.mTimeStamp = mElapsedMilliseconds;
//    mAngSpeed.mTimeStamp = mElapsedMilliseconds;
//    mForceMoment.mTimeStamp = mElapsedMilliseconds;
    mPosition.mValid = 1;
    mAttitude.mValid = 1;
    mLinearSpeed.mValid = 1;
    mAngSpeed.mValid = 1;
    mForceMoment.mValid = 1;
//	//
//	// show state
//	//
//	char strPos[256],strAtt[256],strVel[256],strOmega[256];
//	mPosition.sprint(strPos);
//	mAttitude.sprint(strAtt);
//	mLinearSpeed.sprint(strVel);
//	mAngSpeed.sprint(strOmega);
////	printf("RigidBodyVesselThread::execute_thread_semantics %s%s%s%s\r\n",strPos,strAtt,strVel,strOmega);fflush(stdout);

}

void RigidBodyVesselThread::build_data(void) {
	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	packet.n=mPosition.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//printf("RigidBodyVesselThread::build_data n %d data %s",packet.n,packet.data);
	packet.n=mAttitude.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//printf("RigidBodyVesselThread::build_data n %d data %s",packet.n,packet.data);
	packet.n=mLinearSpeed.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//printf("RigidBodyVesselThread::build_data n %d data %s",packet.n,packet.data);
	packet.n=mAngSpeed.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//printf("RigidBodyVesselThread::build_data n %d data %s",packet.n,packet.data);
	packet.n=mForceMoment.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//printf("RigidBodyVesselThread::build_data n %d data %s",packet.n,packet.data);

}

void get_force_moment(ForceMoment &tau,RigidBodyCmd cmd) {
	tau.X=cmd.X;
	tau.Y=cmd.Y;
	tau.Z=cmd.Z;
	tau.K=cmd.K;
	tau.M=cmd.M;
	tau.N=cmd.N;
}

