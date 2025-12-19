/*
 * GuidanceControlReference.cpp
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
 
#include "GuidanceControlReference.h"
#include <stdio.h>
 
GuidanceReference::GuidanceReference(string name):TimeStampedData(name) {
	mPacketCode=GUIDANCE_REFERENCE_CODE;
	reset();
}

GuidanceReference::GuidanceReference(void):TimeStampedData() {
	mPacketCode=GUIDANCE_REFERENCE_CODE;
	reset();
}


void GuidanceReference::reset(void) {
	// local Earth-fixed NED frame coordinates
	lat0=0.; // [deg.dddddddd]
	lon0=0.; // [deg.dddddddd]
	// Line-Following: latitude, longitude, and orientation of a reference line point
	latL=0.; // [deg.dddddddd]
	lonL=0.; // [deg.dddddddd]
	gammaL=0.; // [deg]
	// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
	latWP=0.; // [deg.dddddddd]
	lonWP=0.; // [deg.dddddddd]
	// KEEP-POINT: reference heading at the desired way-point
	psiWP=0.; // [deg]
	// desired linear velocity in vehicle-fixed frame
	uBar=0.; // [m/s]
}

GuidanceReference & GuidanceReference::operator = (const GuidanceReference &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// local Earth-fixed NED frame coordinates
	lat0=obj.lat0;
	lon0=obj.lon0;
	// Line-Following: latitude, longitude, and orientation of a reference line point
	latL=obj.latL;
	lonL=obj.lonL;
	gammaL=obj.gammaL;
	// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
	latWP=obj.latWP;
	lonWP=obj.lonWP;
	// KEEP-POINT: reference heading at the desired way-point
	psiWP=obj.psiWP;
	// desired linear velocity in vehicle-fixed frame
	uBar=obj.uBar;
	// return a reference to myself
	return *this;
}

void GuidanceReference::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_latitude0,"
			"%s_longitude0,"
			"%s_latitudeL,"
			"%s_longitudeL,"
			"%s_orientationL,"
			"%s_latitudeWP,"
			"%s_longitudeWP,"
			"%s_orientationWP,"
			"%s_surge_rate_fore,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void GuidanceReference::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"latitude,"								//CF
			"longitude,"							//CF
			"latitude,"								//CF
			"longitude,"							//CF
			"platform_orientation,"					//CF
			"latitude,"								//CF
			"longitude,"							//CF
			"platform_orientation,"					//CF
			"platform_surge_rate_fore,"				//CF
			,strTS
			);
	strcpy(s,str);
}


int GuidanceReference::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// local Earth-fixed NED frame coordinates
			"%.8lf %.8lf "
			// Line-Following: latitude, longitude, and orientation of a reference line point
			"%.8lf %.8lf %.2lf "
			// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
			"%.8lf %.8lf "
			// KEEP-POINT: reference heading at the desired way-point
			"%.2lf "
			// desired linear velocity in vehicle-fixed frame
			"%.2lf "
			" ",
			// time stamp
			str1,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// Line-Following: latitude, longitude, and orientation of a reference line point
			latL,lonL,gammaL,
			// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
			latWP,lonWP,
			// KEEP-POINT: reference heading at the desired way-point
			psiWP,
			// desired linear velocity in vehicle-fixed frame
			uBar
			);
	strcpy(s,str);
	return n;
}


void GuidanceReference::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"LF,%s"
			// local Earth-fixed NED frame coordinates
			"%.8lf,%.8lf,"
			// Line-Following: latitude, longitude, and orientation of a reference line point
			"%.8lf,%.8lf,%.2lf,"
			// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
			"%.8lf,%.8lf,"
			// KEEP-POINT: reference heading at the desired way-point
			"%.2lf,"
			// desired linear velocity in vehicle-fixed frame
			"%.2lf"
			",",
			// time stamp
			str1,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// Line-Following: latitude, longitude, and orientation of a reference line point
			latL,lonL,gammaL,
			// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
			latWP,lonWP,
			// KEEP-POINT: reference heading at the desired way-point
			psiWP,
			// desired linear velocity in vehicle-fixed frame
			uBar
			);
	strcpy(s,str);
}

void GuidanceReference::sread(char *s) {
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
			// local Earth-fixed NED frame coordinates
			" %lf %lf"
			// Line-Following: latitude, longitude, and orientation of a reference line point
			" %lf %lf %lf"
			// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
			" %lf %lf"
			// KEEP-POINT: reference heading at the desired way-point
			" %lf"
			// desired linear velocity in vehicle-fixed frame
			" %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// local Earth-fixed NED frame coordinates
			&lat0,&lon0,
			// Line-Following: latitude, longitude, and orientation of a reference line point
			&latL,&lonL,&gammaL,
			// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
			&latWP,&lonWP,
			// KEEP-POINT: reference heading at the desired way-point
			&psiWP,
			// desired linear velocity in vehicle-fixed frame
			&uBar
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}







AutopilotReference::AutopilotReference(string name):TimeStampedData(name) {
	mPacketCode=AUTOPILOT_REFERENCE_CODE;
	reset();
}

AutopilotReference::AutopilotReference(void):TimeStampedData() {
	mPacketCode=AUTOPILOT_REFERENCE_CODE;
	reset();
}


void AutopilotReference::reset(void) {
	// AUTO-HEADING: reference heading
	psi=0.; // [deg]
	// AUTO-COURSE: reference course
	chi=0.; // [deg]
	// desired linear velocity in vehicle-fixed frame
	uBar=0.; // [m/s]
}

AutopilotReference & AutopilotReference::operator = (const AutopilotReference &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// AUTO-HEADING: reference heading
	psi=obj.psi;
	// AUTO-COURSE: reference course
	chi=obj.chi;
	// desired linear velocity in vehicle-fixed frame
	uBar=obj.uBar;
	// return a reference to myself
	return *this;
}

void AutopilotReference::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_orientation,"
			"%s_course,"
			"%s_surge_rate_fore,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void AutopilotReference::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"platform_yaw,"							//CF
			"platform_course,"						//CF
			"platform_surge_rate_fore,"				//CF
			,strTS
			);
	strcpy(s,str);
}


int AutopilotReference::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// AUTO-HEADING: reference heading
			"%.2lf "
			// AUTO-COURSE: reference course
			"%.2lf "
			// desired linear velocity in vehicle-fixed frame
			"%.2lf "
			" ",
			// time stamp
			str1,
			// AUTO-HEADING: reference heading
			psi,
			// AUTO-COURSE: reference course
			chi,
			// desired linear velocity in vehicle-fixed frame
			uBar
			);
	strcpy(s,str);
	return n;
}


void AutopilotReference::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"LF,%s"
			// AUTO-HEADING: reference heading
			"%.2lf,"
			// AUTO-COURSE: reference course
			"%.2lf,"
			// desired linear velocity in vehicle-fixed frame
			"%.2lf"
			",",
			// time stamp
			str1,
			// AUTO-HEADING: reference heading
			psi,
			// AUTO-COURSE: reference course
			chi,
			// desired linear velocity in vehicle-fixed frame
			uBar
			);
	strcpy(s,str);
}

void AutopilotReference::sread(char *s) {
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
			// AUTO-HEADING: reference heading
			" %lf"
			// AUTO-COURSE: reference course
			" %lf"
			// desired linear velocity in vehicle-fixed frame
			" %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// AUTO-HEADING: reference heading
			&psi,
			// AUTO-COURSE: reference course
			&chi,
			// desired linear velocity in vehicle-fixed frame
			&uBar
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}





















ControlReference::ControlReference(string name):TimeStampedData(name) {
	mPacketCode=CONTROL_REFERENCE_CODE;
	reset();
}

ControlReference::ControlReference(void):TimeStampedData() {
	mPacketCode=CONTROL_REFERENCE_CODE;
	reset();
}

void ControlReference::reset(void) {
	// surge, sway, yawrate
	u=0.; // [m/s]
	v=0.; // [m/s]
	r=0.; // [deg/s]
}

ControlReference & ControlReference::operator = (const ControlReference &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// surge, sway, yawrate
	u=obj.u; // [m/s]
	v=obj.v; // [m/s]
	r=obj.r; // [deg/s]
	// return a reference to myself
	return *this;
}

void ControlReference::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_surge_rate_fore,"
			"%s_sway_rate_starboard,"
			"%s_yaw_rate,"
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void ControlReference::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"platform_surge_rate_fore,"				//CF
			"platform_sway_rate_starboard,"			// CF
			"platform_yaw_rate,"					// CF
			,strTS
			);
	strcpy(s,str);
}

int ControlReference::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// surge, sway, yawrate
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// surge, sway, yawrate
			u,v,r
			);
	strcpy(s,str);
	return n;
}

void ControlReference::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"LF,%s"
			// surge, sway, yawrate
			"%.2lf,%.2lf,%.2lf"
			",",
			// time stamp
			str1,
			// surge, sway, yawrate
			u,v,r
			);
	strcpy(s,str);
}

void ControlReference::sread(char *s) {
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
			// surge, sway, yawrate
			" %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// surge, sway, yawrate
			&u,&v,&r
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}



