/*
 * VehicleDvlData.cpp
 *
 *  Created on: 13 January 2025
 *      Author: massimo
 */
 
#include "VehicleDvlData.h"

VehicleDvlData::VehicleDvlData(string name):LogicalDvlData(name) {
	mPacketCode=VEHICLE_DVL_CODE;
	reset();
}

VehicleDvlData::VehicleDvlData(void):LogicalDvlData() {
	// TODO Auto-generated constructor stub
	mPacketCode=VEHICLE_DVL_CODE;
	reset();
}

void VehicleDvlData::reset(void) {
	LogicalDvlData::reset();
	mPose.reset();
}

VehicleDvlData & VehicleDvlData::operator = (const VehicleDvlData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	LogicalDvlData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void VehicleDvlData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	LogicalDvlData::sprint_csv_log_name(strTS);
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

void VehicleDvlData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	LogicalDvlData::sprint_csv_long_name(strTS);
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

int VehicleDvlData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	LogicalDvlData::sprint(str1);
	n=sprintf(str,"%s"
			// linear pose
			"%.2lf %.2lf %.2lf "
			// angular pose
			"%.2lf %.2lf %.2lf "
			" ",
			// logical GNSS velocity data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
	return n;
}

void VehicleDvlData::sprint_csv(char *s) {
	char str[2048],str1[256];

	LogicalDvlData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear pose
			"%.2lf,%.2lf,%.2lf,"
			// angular pose
			"%.2lf,%.2lf,%.2lf,",
			// logical GNSS velocity data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
}


void VehicleDvlData::sread(char *s) {
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
			// velocity
			"%lf %lf %lf "
			// range from bottom
			"%lf %lf %lf %lf"
			// velocity stdev
			" %lf %lf %lf"
			// range from bottom stdev
			" %lf"
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
			// velocity
			&u,&v,&w,
			// range from bottom
			&rho0,&rho1,&rho2,&rho3,
			// velocity stdev
			&uStd,&vStd,&wStd,
			// angular velocity stdev
			&rhoStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void VehicleDvlData::assign(LogicalDvlData logicalDvlData)  {
	// velocity
	u=logicalDvlData.u;
	v=logicalDvlData.v;
	w=logicalDvlData.w;
	// range from bottom
	rho0=logicalDvlData.rho0;
	rho1=logicalDvlData.rho1;
	rho2=logicalDvlData.rho2;
	rho3=logicalDvlData.rho3;
	// velocity stdev
	uStd=logicalDvlData.uStd;
	vStd=logicalDvlData.vStd;
	wStd=logicalDvlData.wStd;
	// range from bottom stdev
	rhoStd=logicalDvlData.rhoStd;
}

void VehicleDvlData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}

