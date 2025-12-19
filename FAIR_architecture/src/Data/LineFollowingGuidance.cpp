/*
 * LineFollowingGuidance.cpp
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
 
#include "LineFollowingGuidance.h"
#include <stdio.h>
 
LineFollowingGuidance::LineFollowingGuidance(string name):TimeStampedData(name) {
	reset();
}

LineFollowingGuidance::LineFollowingGuidance(void):TimeStampedData() {
	reset();
}


void LineFollowingGuidance::reset(void) {
	latL=0;
	lonL=0;
	lat0=0;
	lon0=0;
	xL=0.;
	yL=0.;
	gammaL=0;
	uLF=0.;
}

LineFollowingGuidance & LineFollowingGuidance::operator = (const LineFollowingGuidance &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// latitude, longitude of a line point
	latL=obj.latL;
	lonL=obj.lonL;
	// local Earth-fixed NED frame coordinates
	lat0=obj.lat0;
	lon0=obj.lon0;
	// position of a line point in the local Earth-fixed NED frame
	xL=obj.xL;
	yL=obj.yL;
	// orientation of the line in the local Earth-fixed NED frame
	gammaL=obj.gammaL;
	// linear velocity in vehicle-fixed frame
	uLF=obj.uLF;
	// return a reference to myself
	return *this;
}

void LineFollowingGuidance::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"%s_latitude,"
			"%s_longitude,"
			"%s_latitude0,"
			"%s_longitude0,"
			"%s_x,"
			"%s_y,"
			"%s_orientation,"
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
			);
	strcpy(s,str);
}


void LineFollowingGuidance::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"latitude,"								//CF
			"longitude,"							//CF
			"latitude,"								//CF
			"longitude,"							//CF
			"projection_x_coordinate,"				//CF
			"projection_y_coordinate,"				//CF
			"platform_yaw,"							//CF
			"platform_surge_rate_fore,"				//CF
			,strTS
			);
	strcpy(s,str);
}


int LineFollowingGuidance::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"LF %s"
			// angles
			"%.8lf %.8lf "
			// local Earth-fixed NED frame coordinates
			"%.8lf %.8lf "
			// position in the localEarth-fixed NED frame
			"%.2lf %.2lf "
			// angles
			"%.2lf "
			// linear velocity in vehicle-fixed frame
			"%.2lf "
			" ",
			// time stamp
			str1,
			// latitude, longitude
			latL,lonL,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			xL,yL,
			// angles
			gammaL, // yaw [deg]
			// linear velocity in vehicle-fixed frame
			uLF
			);
	strcpy(s,str);
	return n;
}


void LineFollowingGuidance::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"LF,%s"
			// latitude, longitude
			"%.8lf,%.8lf,"
			// local Earth-fixed NED frame coordinates
			"%.8lf,%.8lf,"
			// position in the localEarth-fixed NED frame
			"%.2lf,%.2lf,"
			// angles
			"%.2lf,"
			// linear velocity in vehicle-fixed frame
			"%.2lf"
			",",
			// time stamp
			str1,
			// latitude, longitude
			latL,lonL,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			xL,yL,
			// angles
			gammaL, // yaw [deg]
			// linear velocity in vehicle-fixed frame
			uLF
			);
	strcpy(s,str);
}

void LineFollowingGuidance::sread(char *s) {
	char str[64];
	char name[64];
	uint8_t valid;
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %lf "			// name timeStamp
			// valid flag
			" %" SCNu8
			// latitude, longitude
			" %lf %lf"
			// local Earth-fixed NED frame coordinates
			" %lf %lf"
			// position in the localEarth-fixed NED frame
			" %lf %lf"
			// angles
			" %lf"
			// linear velocity in vehicle-fixed frame
			" %lf",
			str,
			// time stamp
			name,&mTimeStamp,
			// valid flag
			&valid,
			// latitude, longitude
			&latL,&lonL,
			// local Earth-fixed NED frame coordinates
			&lat0,&lon0,
			// position in the localEarth-fixed NED frame
			&xL,&yL,
			// angles
			&gammaL, // yaw [deg]
			// linear velocity in vehicle-fixed frame
			&uLF // surge [m/s]
			);
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}


bool LineFollowingGuidance::fread(string fileName) {
	FILE *f;

	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: bool LineFollowingGuidance::fread(string fileName) - %s not found!\n",fileName.c_str());
		return false;
	}

	char str[256];
	// local Earth-fixed NED frame: latitude, longitude
	fscanf(f,"%s %lf",str,&lat0);
    if(strcmp(str,"lat0")!=0)
    {
        printf("LineFollowingGuidance::fread lat0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&lon0);
    if(strcmp(str,"lon0")!=0)
    {
        printf("LineFollowingGuidance::fread lon0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // position in the localEarth-fixed NED frame
	fscanf(f,"%s %lf",str,&xL);
    if(strcmp(str,"xL")!=0)
    {
        printf("LineFollowingGuidance::fread xL expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&yL);
    if(strcmp(str,"yL")!=0)
    {
        printf("LineFollowingGuidance::fread yL expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // compute (latitude,longitude)
    lat_lon_plus_x_y(latL,lonL,lat0,lon0,xL,yL);
    // angles: yaw
	fscanf(f,"%s %lf",str,&gammaL);
    if(strcmp(str,"gammaL")!=0)
    {
        printf("LineFollowingGuidance::fread gammaL expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // linear velocity in vehicle-fixed frame
	fscanf(f,"%s %lf",str,&uLF);
    if(strcmp(str,"uLF")!=0)
    {
        printf("LineFollowingGuidance::fread uLF expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

	fclose(f);
	return true;

}




