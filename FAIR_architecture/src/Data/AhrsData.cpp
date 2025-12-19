/*
 * AhrsData.cpp
 *
 *  Created on: 22 October 2023
 *      Author: massimo
 */
 
 #include "AhrsData.h"

AhrsData::AhrsData(string name):TimeStampedData(name) {
	mPacketCode=AHRS_CODE;
	reset();
}

AhrsData::AhrsData(void):TimeStampedData() {
	// TODO Auto-generated constructor stub
	mPacketCode=AHRS_CODE;
	reset();
}

void AhrsData::reset(void) {
	phi=0; // roll [deg]
	theta=0; // pitch [deg]
	psi=0; // yaw [deg]
	uDot=0; // [m/s^2]
	vDot=0; // [m/s^2]
	wDot=0; // [m/s^2]
	p=0; // roll rate [deg/s]
	q=0; // pitch rate [deg/s]
	r=0; // yaw rate [deg/s]
	magnX=0; // [Gauss]
	magnY=0; // [Gauss]
	magnZ=0; // [Gauss]
	u=0; // [m/s]
	v=0; // [m/s]
	w=0; // [m/s]
}

AhrsData & AhrsData::operator = (const AhrsData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// angles
	phi=obj.phi;
	theta=obj.theta;
	psi=obj.psi;
	// linear accelerations
	uDot=obj.uDot;
	vDot=obj.vDot;
	wDot=obj.wDot;
	// angular velocity
	p=obj.p;
	q=obj.q;
	r=obj.r;
	// magnetic components
	magnX=obj.magnX;
	magnY=obj.magnY;
	magnZ=obj.magnZ;
	// linear velocity
	u=obj.u;
	v=obj.v;
	w=obj.w;
	// return a reference to myself
	return *this;
}


void AhrsData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_roll,"					// CF
			"%s_pitch,"					// CF
			"%s_yaw,"					// CF
			"%s_surge_acceleration,"   	//No CF
			"%s_sway_acceleration,"		//No CF
			"%s_heave_acceleration,"	//No CF
			"%s_roll_rate,"				// CF
			"%s_pitch_rate,"			// CF
			"%s_yaw_rate,"				// CF
			"%s_magnX,"					//No CF	(RF)
			"%s_magnY,"					//No CF	(RF)
			"%s_magnZ,"					//No CF	(RF)
			"%s_surge_rate_fore,"		// CF
			"%s_sway_rate_starboard,"	// CF
			"%s_heave_rate_down,",		// CF
			strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
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

void AhrsData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"platform_roll_starboard_down,"								// CF
			"platform_pitch_fore_up,"								// CF
			"platform_yaw_fore_starboard,"									// CF
			"platform_surge_acceleration_fore,"				// No CF
			"platform_sway_acceleration_starboard,"			// No CF
			"platform_heave_acceleration_down,"				// No CF
			"platform_roll_rate_starboard_down,"							// CF
			"platform_pitch_rate_fore_up,"							// CF
			"platform_yaw_rate_fore_starboard,"							// CF
			"magnetic_field_fore,"							// No CF  	(RF)
			"magnetic_field,_starboard"						// No CF	(RF)
			"magnetic_field_down,"							// No CF	(RF)
			"platform_surge_rate_fore,"						// CF
			"platform_sway_rate_starboard,"					// CF
			"platform_heave_rate_down,"						// CF
			,strTS);
	strcpy(s,str);
}

int AhrsData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// angles
			"%.2lf %.2lf %.2lf "
			// linear accelerations
			"%.2lf %.2lf %.2lf "
			// angular velocity
			"%.2lf %.2lf %.2lf "
			// magnetic components
			"%lf %lf %lf "
			// linear velocity
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// angles
			phi,theta,psi,
			// linear accelerations
			uDot,vDot,wDot,
			// angular velocity
			p,q,r,
			// magnetic components
			magnX,magnY,magnZ,
			// linear velocity
			u,v,w
			);
	strcpy(s,str);
	return n;
}

void AhrsData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// angles
			"%.2lf,%.2lf,%.2lf,"
			// linear accelerations
			"%.2lf,%.2lf,%.2lf,"
			// angular velocity
			"%.2lf,%.2lf,%.2lf,"
			// magnetic components
			"%lf,%lf,%lf,"
			// linear velocity
			"%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// angles
			phi,theta,psi,
			// linear accelerations
			uDot,vDot,wDot,
			// angular velocity
			p,q,r,
			// magnetic components
			magnX,magnY,magnZ,
			// linear velocity
			u,v,w
			);
	strcpy(s,str);
}


void AhrsData::sread(char *s) {
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
			&u,&v,&w
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}
