/*
 * LinearVelocity.cpp
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
 
#include "LinearVelocity.h"
#include <stdio.h>
 
LinearVelocity::LinearVelocity(string name):TimeStampedData(name) {
	mPacketCode=LINEAR_VELOCITY_CODE;
	reset();
	mAttitude=NULL;
}

LinearVelocity::LinearVelocity(void):TimeStampedData() {
	mPacketCode=LINEAR_VELOCITY_CODE;
	reset();
	mAttitude=NULL;
}


void LinearVelocity::reset(void) {
	xDot=0;
	yDot=0;
	zDot=0;
	groundSpeed=0;
	course=0;
	xCdot=0;
	yCdot=0;
	u=0;
	v=0;
	w=0;
	ur=0;
	vr=0;
}

LinearVelocity & LinearVelocity::operator = (const LinearVelocity &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// linear velocity and speed (NED)
	xDot=obj.xDot;
	yDot=obj.yDot;
	zDot=obj.zDot;
	groundSpeed=obj.groundSpeed;
	course=obj.course;
	// sea current
	xCdot=obj.xCdot;
	yCdot=obj.yCdot;
	// linear velocity in vehicle-fixed frame
	u=obj.u;
	v=obj.v;
	w=obj.w;
	// linear velocity with respect to the water in vehicle-fixed frame
	ur=obj.ur;
	vr=obj.vr;

	mAttitude=obj.mAttitude;
	// return a reference to myself
	return *this;
}

void LinearVelocity::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_velocity_North,"
			"%s_velocity_East,"
			"%s_velocity_Down,"
			"%s_ground_speed,"
			"%s_course,"
			"%s_sea_current_North,"
			"%s_sea_current_East,"
			"%s_sea_current_Down,"
			"%s_surge_rate_fore,"
			"%s_sway_rate_starboard,"
			"%s_heave_rate_down,"
			"%s_surge_velocity_water,"
			"%s_sway_velocity_water,"
			"%s_heave_velocity_water,"
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
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}


void LinearVelocity::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
            "platform_northward_velocity,"				// No CF	(RF)
			"platform_eastward_velocity,"				// No CF	(RF)
			"platform_downdown_velocity,"				// No CF	(RF)
            "platform_speed_wrt_ground,"     		// CF
			"platform_course,"						// CF
            "northward_sea_water_velocity,"			// CF
            "eastward_sea_water_velocity,"			// CF
			"downward_sea_water_velocity,"			// No CF
			"platform_surge_rate_fore,"				// CF
			"platform_sway_rate_starboard,"			// CF
			"platform_heave_rate_down,"				// CF
            "platform_surge_rate_fore_wrt_sea_water,"	// No CF
            "platform_sway_rate_starboard_wrt_sea_water,"		// No CF
			"platform_heave_rate_down_wrt_sea_water,"	// No CF
			,strTS
			);
	strcpy(s,str);
}


int LinearVelocity::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// linear velocity and speed (NED)
			"%.2lf %.2lf %.2lf "
			"%.2lf %.2lf "
			// sea current
			"%.2lf %.2lf %.2lf "
			// linear velocity in vehicle-fixed frame
			"%.2lf %.2lf %.2lf "
			// linear velocity with respect to the water in vehicle-fixed frame
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// linear velocity and speed (NED)
			xDot,yDot,zDot,
			groundSpeed,course,
			// sea current
			xCdot,yCdot,zCdot,
			// linear speed in vehicle-fixed frame
			u,v,w,
			// linear speed with respect to the water in vehicle-fixed frame
			ur,vr,wr
			);
	strcpy(s,str);
	return n;
}


void LinearVelocity::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear speed (NED)
			"%.2lf,%.2lf,%.2lf,"
			"%.2lf,%.2lf,"
			// sea current
			"%.2lf,%.2lf,%.2lf,"
			// linear speed in vehicle-fixed frame
			"%.2lf,%.2lf,%.2lf,"
			// linear speed with respect to the water in vehicle-fixed frame
			"%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// linear speed (NED)
			xDot,yDot,zDot,
			groundSpeed,course,
			// sea current
			xCdot,yCdot,zCdot,
			// linear speed in vehicle-fixed frame
			u,v,w,
			// linear speed with respect to the water in vehicle-fixed frame
			ur,vr,wr
			);
	strcpy(s,str);
}

void LinearVelocity::sread(char *s) {
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
			// linear velocity and speed (NED)
			" %lf %lf %lf"
			" %lf %lf"
			// sea current
			" %lf %lf %lf"
			// linear velocity in vehicle-fixed frame
			" %lf %lf %lf"
			// linear velocity with respect to the water in vehicle-fixed frame
			" %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// linear velocity and speed (NED)
			&xDot,&yDot,&zDot,
			&groundSpeed,&course,
			// sea current
			&xCdot,&yCdot,&zCdot,
			// linear speed in vehicle-fixed frame
			&u,&v,&w,
			// linear speed with respect to the water in vehicle-fixed frame
			&ur,&vr,&wr
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}



bool LinearVelocity::fread(FILE *f) {
	char str[256];
    // linear speed with respect to the water in vehicle-fixed frame
	fscanf(f,"%s %lf",str,&ur);
    if(strcmp(str,"ur")!=0)
    {
        printf("LinearVelocity::fread ur expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&vr);
    if(strcmp(str,"vr")!=0)
    {
        printf("LinearVelocity::fread vr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&wr);
    if(strcmp(str,"wr")!=0)
    {
        printf("LinearVelocity::fread wr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // sea current
	fscanf(f,"%s %lf",str,&xCdot);
    if(strcmp(str,"xCdot")!=0)
    {
        printf("LinearVelocity::fread xCdot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&yCdot);
    if(strcmp(str,"yCdot")!=0)
    {
        printf("LinearVelocity::fread yCdot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&zCdot);
    if(strcmp(str,"zCdot")!=0)
    {
        printf("LinearVelocity::fread zCdot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // compute linear speed (NED)
    compute_NED_velocity_speed_from_body_relative_speed_and_sea_current(mAttitude);
    // compute linear speed in vehicle-fixed frame
    compute_body_velocity_from_NED_velocity(mAttitude);

	fclose(f);
	return true;

}

void LinearVelocity::compute_speed_from_velocity(void) {
//	speed=sqrt(xDot*xDot+yDot*yDot+zDot*zDot);
	groundSpeed=sqrt(xDot*xDot+yDot*yDot);
	course=rad2deg(atan2(yDot,xDot));
}

void LinearVelocity::compute_NED_velocity_speed_from_body_relative_speed_and_sea_current(AngularPosition *attitude) {
    Vector xDotVector;
    Vector xCdotVector(xCdot,yCdot,zCdot);
    Vector urVector(ur,vr,wr);
    Matrix3x3 Rzyx;
    phi_theta_psi_2_Rzyx(Rzyx,attitude->phi,attitude->theta,attitude->psi);
    xDotVector=Rzyx*urVector+xCdotVector;
    xDot=xDotVector[0];
    yDot=xDotVector[1];
    zDot=xDotVector[2];
    compute_speed_from_velocity();
}

void LinearVelocity::compute_body_velocity_from_NED_velocity(AngularPosition *attitude) {
	Vector xDotVector(xDot,yDot,zDot);
	Matrix3x3 Rzyx;
	phi_theta_psi_2_Rzyx(Rzyx,attitude->phi,attitude->theta,attitude->psi);
	Vector uVector;
	uVector=Rzyx.t()*xDotVector;
	u=uVector[0];
	v=uVector[1];
	w=uVector[2];
}




