/*
 * VehicleGnssPositionData.cpp
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
 
#include "VehicleGnssPositionData.h"

VehicleGnssPositionData::VehicleGnssPositionData(string name):LogicalGnssPositionData(name) {
	mPacketCode=VEHICLE_GNSS_POSITION_CODE;
	reset();
}

VehicleGnssPositionData::VehicleGnssPositionData(void):LogicalGnssPositionData() {
	// TODO Auto-generated constructor stub
	mPacketCode=VEHICLE_GNSS_POSITION_CODE;
	reset();
}

void VehicleGnssPositionData::reset(void) {
	LogicalGnssPositionData::reset();
	mPose.reset();
}

VehicleGnssPositionData & VehicleGnssPositionData::operator = (const VehicleGnssPositionData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	LogicalGnssPositionData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void VehicleGnssPositionData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	LogicalGnssPositionData::sprint_csv_log_name(strTS);
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

void VehicleGnssPositionData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	LogicalGnssPositionData::sprint_csv_long_name(strTS);
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

int VehicleGnssPositionData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	LogicalGnssPositionData::sprint(str1);
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

void VehicleGnssPositionData::sprint_csv(char *s) {
	char str[2048],str1[256];

	LogicalGnssPositionData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear pose
			"%.2lf,%.2lf,%.2lf,"
			// angular pose
			"%.2lf,%.2lf,%.2lf,",
			// logical GNSS position data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
}


void VehicleGnssPositionData::sread(char *s) {
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
			// date, time
			" %" SCNu64 " %lf"
			// latitude, longitude
			" %lf %lf"
			// GPS height
			" %lf %lf"
			// fix quality, n satellites, HDOP
			" %" SCNu8 " %" SCNu8 " %lf"
			// latitude,longitude stdev
			" %lf %lf"
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
			// date and time
			&date,&time,
			// latitude, longitude
			&latitude,&longitude,
			// GPS height
			&heightAboveEllipsoid,&heightAboveMSL,
			// fix quality, n satellites, HDOP
			&fixQuality,&nSatellites,&hdop,
			// latitude,longitude stdev
			&latStd,&lonStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void VehicleGnssPositionData::assign(LogicalGnssPositionData logicalGnssPositionData)  {
	// date, time
	date=logicalGnssPositionData.date; // yyyymmdd : date = 10000*yyyy + 100*mm + dd (ZDA, obtained by...)
	time=logicalGnssPositionData.time; // hhmmss.ss in UTC (GGA, ZDA)
	// latitude, longitude
	latitude=logicalGnssPositionData.latitude; // [deg.dddddddd] (GGA)
	longitude=logicalGnssPositionData.longitude; // [deg.dddddddd] (GGA)
	// GPS height
	heightAboveEllipsoid=logicalGnssPositionData.heightAboveEllipsoid; // [m] (GGA: Ellipsoid altitude = MSL Altitude + Geoid Separation)
	heightAboveMSL=logicalGnssPositionData.heightAboveMSL; // [m] (GGA)
	// fix quality, n satellites, HDOP
	fixQuality=logicalGnssPositionData.fixQuality; // (GGA)
	nSatellites=logicalGnssPositionData.nSatellites; // GGA)
	hdop=logicalGnssPositionData.hdop; // (GGA)
	// latitude,longitude stdev
	latStd=logicalGnssPositionData.latStd;
	lonStd=logicalGnssPositionData.lonStd;
}

void VehicleGnssPositionData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}




