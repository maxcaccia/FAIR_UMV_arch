/*
 * VehicleAhrsData.cpp
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
 
#include "VehicleAhrsData.h"

VehicleAhrsData::VehicleAhrsData(string name):LogicalAhrsData(name) {
	mPacketCode=VEHICLE_AHRS_CODE;
	reset();
}

VehicleAhrsData::VehicleAhrsData(void):LogicalAhrsData() {
	// TODO Auto-generated constructor stub
	mPacketCode=VEHICLE_AHRS_CODE;
	reset();
}

void VehicleAhrsData::reset(void) {
	LogicalAhrsData::reset();
	mPose.reset();
}

VehicleAhrsData & VehicleAhrsData::operator = (const VehicleAhrsData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	LogicalAhrsData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void VehicleAhrsData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	LogicalAhrsData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_dx,"					// CF
			"%s_dy,"					// CF
			"%s_dz,"					// CF
			"%s_droll,"				// CF
			"%s_dpitch,"			// CF
			"%s_dyaw_,",				// CF
			strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void VehicleAhrsData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	LogicalAhrsData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"device_x_displacement,"								// CF
			"device_y_displacement,"								// CF
			"device_z_displacement,"									// CF
			"device_roll_displacement,"							// CF
			"device_pitch_displacement,"							// CF
			"device_yaw_displacement,"							// CF
			,strTS);
	strcpy(s,str);
}

int VehicleAhrsData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	LogicalAhrsData::sprint(str1);
	n=sprintf(str,"%s"
			// linear pose
			"%.2lf %.2lf %.2lf "
			// angular pose
			"%.2lf %.2lf %.2lf "
			" ",
			// logical ahrs data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
	return n;
}

void VehicleAhrsData::sprint_csv(char *s) {
	char str[2048],str1[256];

	LogicalAhrsData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear pose
			"%.2lf,%.2lf,%.2lf,"
			// angular pose
			"%.2lf,%.2lf,%.2lf,",
			// logical ahrs data
			str1,
			// angles stdev
			mPose.x,mPose.y,mPose.z,
			// angular velocity stdev
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
}


void VehicleAhrsData::sread(char *s) {
	char packetCode[64];
	char name[64];
	uint8_t valid;
	sscanf(s,// packet_code
			"%s "
			// name
			"%s "
			// time stamp
			"%lf "
			// valid flag
			" %" SCNu8
			// angles
			" %lf %lf %lf"
			// linear accelerations
			" %lf %lf %lf"
			// angular velocity
			" %lf %lf %lf"
			// magnetic components
			" %lf %lf %lf"
			// linear velocity
			" %lf %lf %lf"
			// angles stdev
			" %lf %lf %lf"
			// angular velocity stdev
			" %lf %lf %lf"
			// linear pose
			" %lf %lf %lf"
			// angular pose
			" %lf %lf %lf"
			"",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// angles
			&phi,&theta,&psi,
			// linear accelerations
			&uDot,&vDot,&wDot,
			// angular velocity
			&p,&q,&r,
			// magnetic components
			&magnX,&magnY,&magnZ,
			// linear velocity
			&u,&v,&w,
			// angles stdev
			&phiStd,&thetaStd,&psiStd,
			// angular velocity stdev
			&pStd,&qStd,&rStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void VehicleAhrsData::assign(LogicalAhrsData logicalAhrsData)  {
	// angles
	phi=logicalAhrsData.phi;
	theta=logicalAhrsData.theta;
	psi=logicalAhrsData.psi;
	// linear accelerations
	uDot=logicalAhrsData.uDot;
	vDot=logicalAhrsData.vDot;
	wDot=logicalAhrsData.wDot;
	// angular velocity
	p=logicalAhrsData.p;
	q=logicalAhrsData.q;
	r=logicalAhrsData.r;
	// magnetic components
	magnX=logicalAhrsData.magnX;
	magnY=logicalAhrsData.magnY;
	magnZ=logicalAhrsData.magnZ;
	// linear velocity
	u=logicalAhrsData.u;
	v=logicalAhrsData.v;
	w=logicalAhrsData.w;
	// angle stdev
	phiStd=logicalAhrsData.phiStd;
	thetaStd=logicalAhrsData.thetaStd;
	psiStd=logicalAhrsData.psiStd;
	// angular velocity stdev
	pStd=logicalAhrsData.pStd;
	qStd=logicalAhrsData.qStd;
	rStd=logicalAhrsData.rStd;
}

void VehicleAhrsData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}




