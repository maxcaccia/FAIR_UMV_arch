//
//  DevicePose.cpp
//  Device pose: handles kinematics of a device mounted on a rigid-body
//
//  Created by Massimo Caccia on 29/01/24.
//

#include "DevicePose.h"

void DevicePose::reset(void) {
	x=0.;
	y=0.;
	z=0.;
	phi=0.;
	theta=0.;
	psi=0.;
}

DevicePose & DevicePose::operator = (const DevicePose &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// device offset in the vehicle-fixed reference frame
	x=obj.x;
	y=obj.y;
	z=obj.z;
	// device yaw-pitch-roll rotations in the vehicle-fixed reference frame
	phi=obj.phi;
	theta=obj.theta;
	psi=obj.psi;
	// return a reference to myself
	return *this;
}

bool DevicePose::fread(FILE *f) {
	char nome[256];
	fscanf(f,"%s %lf",nome,&x);
	if(strcmp(nome,"x")!=0) {
		printf("ERROR: DevicePose::fread - read %s while expected x\n",nome);
		return(false);
	}
	fscanf(f,"%s %lf",nome,&y);
	if(strcmp(nome,"y")!=0) {
		printf("ERROR: DevicePose::fread - read %s while expected y\n",nome);
		return(false);
	}
	fscanf(f,"%s %lf",nome,&z);
	if(strcmp(nome,"z")!=0) {
		printf("ERROR: DevicePose::fread - read %s while expected z\n",nome);
		return(false);
	}
	fscanf(f,"%s %lf",nome,&phi);
	if(strcmp(nome,"phi")!=0) {
		printf("ERROR: DevicePose::fread - read %s while expected phi\n",nome);
		return(false);
	}
	fscanf(f,"%s %lf",nome,&theta);
	if(strcmp(nome,"theta")!=0) {
		printf("ERROR: DevicePose::fread - read %s while expected theta\n",nome);
		return(false);
	}
	fscanf(f,"%s %lf",nome,&psi);
	if(strcmp(nome,"psi")!=0) {
		printf("ERROR: DevicePose::fread - read %s while expected psi\n",nome);
		return(false);
	}
	return true;
}

void compute_device_pose(
		LinearPosition &devPos,AngularPosition &devAttitude,LinearVelocity &devLinVel,AngularVelocity &devAngVel,
		DevicePose mPose,
		LinearPosition rbPos,AngularPosition rbAttitude,LinearVelocity rbLinVel,AngularVelocity rbAngVel) {
//		Kinematics &mDev,DevicePose mPose,Kinematics mRB) {
//	printf("DevicePose.h - void compute_device_pose() %s\n",mName.c_str());
	//
	// compute device pose with respect to a NED frame instantaneously fixed to the vehicle
	//
	//
	// reference frames
	//	E: NED frame positioned in rigid-body centre of mass at a given time
	//	B: surge-sway-heave rigid body -fixed frame
	//	D: device -fixed frame
	//

	//	p_B : device pose p in <B>
	Vector p_B(mPose.x,mPose.y,mPose.z);
	//	R_EB : rotation matrix from <E> to <B>
	Matrix3x3 R_EB;
	phi_theta_psi_2_Rzyx(R_EB,rbAttitude.phi,rbAttitude.theta,rbAttitude.psi);
	//	p_E : device pose in <E>
	Vector p_E;
	p_E = R_EB*p_B;
	//	omega_B : rigid-body angular velocity in <B>
	Vector omegaDeg_B(rbAngVel.p,rbAngVel.q,rbAngVel.r);
	Vector omega_B;
	omega_B = omegaDeg_B*(M_PI/180.);
	//	omega_B : rigid-body angular velocity in <E>
	Vector omega_E;
	omega_E=R_EB*omega_B;
	//	v0_E : rigid-body linear velocity in <E>
	Vector v0_E(rbLinVel.xDot,rbLinVel.yDot,rbLinVel.zDot);
	//	v_E : device linear velocity in <E>
	Vector v_E;
	v_E = v0_E + omega_E%p_E;
	//	R_BD : rotation matrix from <B> to <D>
	Matrix3x3 R_BD;
	phi_theta_psi_2_Rzyx(R_BD,mPose.phi,mPose.theta,mPose.psi);
	//	R_ED : rotation matrix from <E> to <D>
	Matrix buffer(3,3);
	buffer=R_EB*R_BD;
	Matrix3x3 R_ED(buffer);
	//	v_D : device linear velocity in <D>
	Vector v_D;
	v_D = R_ED.t()*v_E;
	//	omega_D : device angular velocity in <D>
	Vector omega_D;
	omega_D = R_ED.t()*omega_E;
	//	v_B : device linear velocity in <B>
	Vector v_B;
	v_B = R_EB.t()*v_E;

	//
	// compute device kinematics
	//
	devPos=rbPos;
	devAttitude=rbAttitude;
	devLinVel=rbLinVel;
	devAngVel=rbAngVel;
//	mDev=mRB;
	// latitude, longitude
	lat_lon_plus_x_y(devPos.latitude,devPos.longitude,rbPos.latitude,rbPos.longitude,p_E[0],p_E[1]);
//	lat_lon_plus_x_y(mDev.latitude,mDev.longitude,mRB.latitude,mRB.longitude,p_E[0],p_E[1]);
	// position in the local Earth-fixed NED frame
	lat_lon_2_x_y(devPos.x,devPos.y,devPos.lat0,devPos.lon0,devPos.latitude,devPos.longitude);
//	lat_lon_2_x_y(mDev.x,mDev.y,mRB.lat0,mRB.lon0,mDev.latitude,mDev.longitude);
	// linear velocity and speed (NED)
	devLinVel.xDot = v_E[0];
	devLinVel.yDot = v_E[1];
	devLinVel.zDot = v_E[2];
	devLinVel.compute_speed_from_velocity();
	// linear velocity in device-fixed frame
	devLinVel.u = v_D[0];
	devLinVel.v = v_D[1];
	devLinVel.w = v_D[2];
	// sea current velocity in device-fixed frame
	Vector vC_E(devLinVel.xCdot,devLinVel.yCdot,devLinVel.zCdot);
	Vector vC_D;
	vC_D=R_ED.t()*vC_E;
	// linear velocity with respect to the water in device-fixed frame
	Vector vr_D;
	vr_D=v_D-vC_D;
	devLinVel.ur=vr_D[0];
	devLinVel.vr=vr_D[1];
	devLinVel.wr=vr_D[2];
	// angular velocity
	devAngVel.p=omega_D[0]*180./M_PI;
	devAngVel.q=omega_D[1]*180./M_PI;
	devAngVel.r=omega_D[2]*180./M_PI;
	// angles
	Rzyx_2_phi_theta_psi(devAttitude.phi,devAttitude.theta,devAttitude.psi,R_ED);
	devAttitude.phi*=180./M_PI;
	devAttitude.theta*=180./M_PI;
	devAttitude.psi*=180./M_PI;

}


void DevicePose::print(void) {
	printf("\tx %.3lf\ty %.3lf\tz %.3lf\n\tphi %.3lf\ttheta %.3lf\tpsi %.3lf\n",
			x,y,z,phi,theta,psi);
	fflush(stdout);
}
