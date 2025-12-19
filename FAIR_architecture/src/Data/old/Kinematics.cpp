/*
 * Kinematics.cpp
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
 
#include "Kinematics.h"
#include <stdio.h>
 
Kinematics::Kinematics(string name):TimeStampedData(name) {
	reset();
}

Kinematics::Kinematics(void):TimeStampedData() {
	reset();
}


void Kinematics::reset(void) {
	latitude=0;
	longitude=0;
	lat0=0;
	lon0=0;
	x=0;
	y=0;
	z=0;
	phi=0;
	theta=0;
	psi=0;
	xDot=0;
	yDot=0;
	zDot=0;
	speed=0;
	groundSpeed=0;
	track=0;
	xCdot=0;
	yCdot=0;
	u=0;
	v=0;
	w=0;
	ur=0;
	vr=0;
	p=0;
	q=0;
	r=0;
}

Kinematics & Kinematics::operator = (const Kinematics &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// latitude, longitude
	latitude=obj.latitude;
	longitude=obj.longitude;
	// local Earth-fixed NED frame coordinates
	lat0=obj.lat0;
	lon0=obj.lon0;
	// position in the localEarth-fixed NED frame
	x=obj.x;
	y=obj.y;
	z=obj.z;
	// angles
	phi=obj.phi;
	theta=obj.theta;
	psi=obj.psi;
	// linear velocity and speed (NED)
	xDot=obj.xDot;
	yDot=obj.yDot;
	zDot=obj.zDot;
	speed=obj.speed;
	groundSpeed=obj.groundSpeed;
	track=obj.track;
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
	// angular velocity
	p=obj.p;
	q=obj.q;
	r=obj.r;
	// return a reference to myself
	return *this;
}

void Kinematics::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"%s_latitude,"
			"%s_longitude,"
			"%s_latitude0,"
			"%s_longitude0,"
			"%s_x,"
			"%s_y,"
			"%s_z,"
			"%s_roll,"
			"%s_pitch,"
			"%s_yaw,"
			"%s_velocity_North,"
			"%s_velocity_East,"
			"%s_velocity_Down,"
			"%s_speed,"
			"%s_ground_speed,"
			"%s_track,"
			"%s_sea_current_North,"
			"%s_sea_current_East,"
			"%s_sea_current_Down,"
			"%s_surge_rate_fore,"
			"%s_sway_rate_starboard,"
			"%s_heave_rate_down,"
			"%s_surge_velocity_water,"
			"%s_sway_velocity_water,"
			"%s_heave_velocity_water,"
			"%s_roll_rate,"
			"%s_pitch_rate,"
			"%s_yaw_rate,"
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


void Kinematics::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"latitude,"								//CF
			"longitude,"							//CF
			"latitude,"								//CF
			"longitude,"							//CF
			"projection_x_coordinate,"				//CF
			"projection_y_coordinate,"				//CF
            "projection_z_coordinate,"				//No CF (RF)
			"platform_roll,"						//CF
			"platform_pitch,"						//CF
			"platform_yaw,"							//CF
            "platform_north_velocity,"				// No CF	(RF)
			"platform_east_velocity,"				// No CF	(RF)
			"platform_down_velocity,"				// No CF	(RF)
            "platform_speed,"                       // No CF                       %
            "platform_speed_wrt_ground,"     		// CF
			"platform_track,"						// No CF	(RF)
            "northward_sea_water_velocity,"			// CF
            "eastward_sea_water_velocity,"			// CF
			"downward_sea_water_velocity,"			// No CF
			"platform_surge_rate_fore,"				// CF
			"platform_sway_rate_starboard,"			// CF
			"platform_heave_rate_down,"				// CF
            "platform_surge_rate_wrt_sea_water,"	// No CF
            "platform_sway_rate_wrt_sea_water,"		// No CF
			"platform_heave_rate_wrt_sea_water,"	// No CF
			"platform_roll_rate,"					// CF
			"platfotm_pitch_rate,"					// CF
			"platform_yaw_rate,"					// CF
			,strTS
			);
	strcpy(s,str);
}


int Kinematics::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"KINEMATICS %s"
			// angles
			"%.8lf %.8lf "
			// local Earth-fixed NED frame coordinates
			"%.8lf %.8lf "
			// position in the localEarth-fixed NED frame
			"%.2lf %.2lf %.2lf "
			// angles
			"%.2lf %.2lf %.2lf "
			// linear velocity and speed (NED)
			"%.2lf %.2lf %.2lf "
			"%.2lf %.2lf %.2lf "
			// sea current
			"%.2lf %.2lf %.2lf "
			// linear velocity in vehicle-fixed frame
			"%.2lf %.2lf %.2lf "
			// linear velocity with respect to the water in vehicle-fixed frame
			"%.2lf %.2lf %.2lf "
			// angular velocity
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			x,y,z,
			// angles
			phi,theta,psi, // yaw [deg]
			// linear velocity and speed (NED)
			xDot,yDot,zDot,
			speed,groundSpeed,track,
			// sea current
			xCdot,yCdot,zCdot,
			// linear speed in vehicle-fixed frame
			u,v,w,
			// linear speed with respect to the water in vehicle-fixed frame
			ur,vr,wr,
			// angular speed
			p,q,r
			);
	strcpy(s,str);
	return n;
}


void Kinematics::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"KINEMATICS,%s"
			// latitude, longitude
			"%.8lf,%.8lf,"
			// local Earth-fixed NED frame coordinates
			"%.8lf,%.8lf,"
			// position in the localEarth-fixed NED frame
			"%.2lf,%.2lf,%.2lf,"
			// angles
			"%.2lf,%.2lf,%.2lf,"
			// linear speed (NED)
			"%.2lf,%.2lf,%.2lf,"
			"%.2lf,%.2lf,%.2lf,"
			// sea current
			"%.2lf,%.2lf,%.2lf,"
			// linear speed in vehicle-fixed frame
			"%.2lf,%.2lf,%.2lf,"
			// linear speed with respect to the water in vehicle-fixed frame
			"%.2lf,%.2lf,%.2lf,"
			// angular speed
			"%.2lf,%.2lf,%.2lf"
			",",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			x,y,z,
			// angles
			phi,theta,psi, // yaw [deg]
			// linear speed (NED)
			xDot,yDot,zDot,
			speed,groundSpeed,track,
			// sea current
			xCdot,yCdot,zCdot,
			// linear speed in vehicle-fixed frame
			u,v,w,
			// linear speed with respect to the water in vehicle-fixed frame
			ur,vr,wr,
			// angular speed
			p,q,r
			);
	strcpy(s,str);
}

void Kinematics::sread(char *s) {
	char str[64];
	char name[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			" %lf %lf"
			// local Earth-fixed NED frame coordinates
			" %lf %lf"
			// position in the localEarth-fixed NED frame
			" %lf %lf %lf"
			// angles
			" %lf %lf %lf"
			// linear velocity and speed (NED)
			" %lf %lf %lf"
			" %lf %lf %lf"
			// sea current
			" %lf %lf %lf"
			// linear velocity in vehicle-fixed frame
			" %lf %lf %lf"
			// linear velocity with respect to the water in vehicle-fixed frame
			" %lf %lf %lf"
			// angular velocity
			" %lf %lf %lf",
			str,
			// time stamp
			name,&mTimeStamp,
			// latitude, longitude
			&latitude,&longitude,
			// local Earth-fixed NED frame coordinates
			&lat0,&lon0,
			// position in the localEarth-fixed NED frame
			&x,&y,&z,
			// angles
			&phi,&theta,&psi, // yaw [deg]
			// linear velocity and speed (NED)
			&xDot,&yDot,&zDot,
			&speed,&groundSpeed,&track,
			// sea current
			&xCdot,&yCdot,&zCdot,
			// linear speed in vehicle-fixed frame
			&u,&v,&w,
			// linear speed with respect to the water in vehicle-fixed frame
			&ur,&vr,&wr,
			// angular speed
			&p,&q,&r
			);
	mName=name;
//	strcpy(s,str);
}






bool Kinematics::fread(string fileName) {
	FILE *f;

	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: bool KinematicsDynamics::fread(string fileName) - %s not found!\n",fileName.c_str());
		return false;
	}

	char str[256];
	// local Earth-fixed NED frame: latitude, longitude
	fscanf(f,"%s %lf",str,&lat0);
    if(strcmp(str,"lat0")!=0)
    {
        printf("KinematicsDynamics::fread lat0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&lon0);
    if(strcmp(str,"lon0")!=0)
    {
        printf("KinematicsDynamics::fread lon0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // position in the localEarth-fixed NED frame
	fscanf(f,"%s %lf",str,&x);
    if(strcmp(str,"x")!=0)
    {
        printf("KinematicsDynamics::fread x expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&y);
    if(strcmp(str,"y")!=0)
    {
        printf("KinematicsDynamics::fread y expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&z);
    if(strcmp(str,"z")!=0)
    {
        printf("KinematicsDynamics::fread z expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // compute (latitude,longitude)
    lat_lon_plus_x_y(latitude,longitude,lat0,lon0,x,y);
    // angles: roll, pitch, yaw
	fscanf(f,"%s %lf",str,&phi);
    if(strcmp(str,"phi")!=0)
    {
        printf("KinematicsDynamics::fread phi expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&theta);
    if(strcmp(str,"theta")!=0)
    {
        printf("KinematicsDynamics::fread theta expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&psi);
    if(strcmp(str,"psi")!=0)
    {
        printf("KinematicsDynamics::fread psi expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // linear speed with respect to the water in vehicle-fixed frame
	fscanf(f,"%s %lf",str,&ur);
    if(strcmp(str,"ur")!=0)
    {
        printf("KinematicsDynamics::fread ur expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&vr);
    if(strcmp(str,"vr")!=0)
    {
        printf("KinematicsDynamics::fread vr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&wr);
    if(strcmp(str,"wr")!=0)
    {
        printf("KinematicsDynamics::fread wr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // angular speed
	fscanf(f,"%s %lf",str,&p);
    if(strcmp(str,"p")!=0)
    {
        printf("KinematicsDynamics::fread p expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&q);
    if(strcmp(str,"q")!=0)
    {
        printf("KinematicsDynamics::fread q expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&r);
    if(strcmp(str,"r")!=0)
    {
        printf("KinematicsDynamics::fread r expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // sea current
	fscanf(f,"%s %lf",str,&xCdot);
    if(strcmp(str,"xCdot")!=0)
    {
        printf("KinematicsDynamics::fread xCdot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&yCdot);
    if(strcmp(str,"yCdot")!=0)
    {
        printf("KinematicsDynamics::fread yCdot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&zCdot);
    if(strcmp(str,"zCdot")!=0)
    {
        printf("KinematicsDynamics::fread zCdot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // compute linear speed (NED)
    compute_NED_velocity_speed_from_body_relative_speed_and_sea_current();
    // compute linear speed in vehicle-fixed frame
    compute_body_velocity_from_NED_velocity();

	fclose(f);
	return true;

}

void Kinematics::compute_speed_from_velocity(void) {
	speed=sqrt(xDot*xDot+yDot*yDot+zDot*zDot);
	groundSpeed=sqrt(xDot*xDot+yDot*yDot);
	track=rad2deg(atan2(yDot,xDot));
}

void Kinematics::compute_NED_velocity_speed_from_body_relative_speed_and_sea_current(void) {
    Vector xDotVector;
    Vector xCdotVector(xCdot,yCdot,zCdot);
    Vector urVector(ur,vr,wr);
    Matrix3x3 Rzyx;
    phi_theta_psi_2_Rzyx(Rzyx,phi,theta,psi);
    xDotVector=Rzyx*urVector+xCdotVector;
    xDot=xDotVector[0];
    yDot=xDotVector[1];
    zDot=xDotVector[2];
    compute_speed_from_velocity();
}

void Kinematics::compute_body_velocity_from_NED_velocity(void) {
	Vector xDotVector(xDot,yDot,zDot);
	Matrix3x3 Rzyx;
	phi_theta_psi_2_Rzyx(Rzyx,phi,theta,psi);
	Vector uVector;
	uVector=Rzyx.t()*xDotVector;
	u=uVector[0];
	v=uVector[1];
	w=uVector[2];
}




