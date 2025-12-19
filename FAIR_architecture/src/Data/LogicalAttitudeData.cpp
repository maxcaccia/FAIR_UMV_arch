/*
 * LogicalAttitudeData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "LogicalAttitudeData.h"

LogicalAttitudeData::LogicalAttitudeData(string name):BodyAttitudeData(name) {
	mPacketCode=LOGICAL_ATTITUDE_CODE;
	reset();
}

LogicalAttitudeData::LogicalAttitudeData(void):BodyAttitudeData() {
	// TODO Auto-generated constructor stub
	mPacketCode=LOGICAL_ATTITUDE_CODE;
	reset();
}

void LogicalAttitudeData::reset(void) {
	BodyAttitudeData::reset();
	mPose.reset();
}

LogicalAttitudeData & LogicalAttitudeData::operator = (const LogicalAttitudeData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	BodyAttitudeData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void LogicalAttitudeData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	BodyAttitudeData::sprint_csv_log_name(strTS);
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

void LogicalAttitudeData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	BodyAttitudeData::sprint_csv_long_name(strTS);
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

int LogicalAttitudeData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	BodyAttitudeData::sprint(str1);
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

void LogicalAttitudeData::sprint_csv(char *s) {
	char str[2048],str1[256];

	BodyAttitudeData::sprint_csv(str1);
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


void LogicalAttitudeData::sread(char *s) {
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
			// angles stdev
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
			// angles stdev
			&phiStd,&thetaStd,&psiStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalAttitudeData::assign(BodyAttitudeData bodyAttitudeData)  {
	// angles
	phi=bodyAttitudeData.phi;
	theta=bodyAttitudeData.theta;
	psi=bodyAttitudeData.psi;
	// angle stdev
	phiStd=bodyAttitudeData.phiStd;
	thetaStd=bodyAttitudeData.thetaStd;
	psiStd=bodyAttitudeData.psiStd;
}

void LogicalAttitudeData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}


//void LogicalAttitudeData::compute_rigid_body_attitude(BodyAttitudeData &rb) {
//	//	R_ED : rotation matrix from <E> to <D>
//	Matrix3x3 R_ED;
//	phi_theta_psi_2_Rzyx(R_ED,phi,theta,psi);
//	//	R_BD : rotation matrix from <B> to <D>
//	Matrix3x3 R_BD;
//	phi_theta_psi_2_Rzyx(R_BD,mPose.phi,mPose.theta,mPose.psi);
//	// R_EB : rotation matrix from <E> to <B>
//	Matrix3x3 R_EB;
//	R_EB = R_ED * R_BD.t();
//	// angles
//	Rzyx_2_phi_theta_psi(rb.phi,rb.theta,rb.psi,R_EB);
//
//}


