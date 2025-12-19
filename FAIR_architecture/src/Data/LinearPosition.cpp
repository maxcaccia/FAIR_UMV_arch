/*
 * LinearPosition.cpp
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
 
#include "LinearPosition.h"
#include <stdio.h>
 
LinearPosition::LinearPosition(string name):TimeStampedData(name) {
	reset();
	mPacketCode=LINEAR_POSITION_CODE;
}

LinearPosition::LinearPosition(void):TimeStampedData() {
	reset();
	mPacketCode=LINEAR_POSITION_CODE;
}


void LinearPosition::reset(void) {
	latitude=0;
	longitude=0;
	lat0=0;
	lon0=0;
	x=0;
	y=0;
	z=0;
}

LinearPosition & LinearPosition::operator = (const LinearPosition &obj) {
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
	// return a reference to myself
	return *this;
}

void LinearPosition::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_latitude,"
			"%s_longitude,"
			"%s_latitude0,"
			"%s_longitude0,"
			"%s_x,"
			"%s_y,"
			"%s_z,"
			,strTS
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


void LinearPosition::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"latitude,"								//CF
			"longitude,"							//CF
			"latitude,"								//CF
			"longitude,"							//CF
			"projection_x_coordinate,"				//CF
			"projection_y_coordinate,"				//CF
            "depth,"								//CF
			,strTS
			);
	strcpy(s,str);
}


int LinearPosition::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// latitude, longitude
			"%.8lf %.8lf "
			// local Earth-fixed NED frame coordinates
			"%.8lf %.8lf "
			// position in the localEarth-fixed NED frame
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			x,y,z
			);
	strcpy(s,str);
	return n;
}


void LinearPosition::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			// latitude, longitude
			"%.8lf,%.8lf,"
			// local Earth-fixed NED frame coordinates
			"%.8lf,%.8lf,"
			// position in the localEarth-fixed NED frame
			"%.2lf,%.2lf,%.2lf,",
			// time stamp
			str1,
			// latitude, longitude
			latitude,longitude,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			x,y,z
			);
	strcpy(s,str);
}

void LinearPosition::sread(char *s) {
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
			// latitude, longitude			
			" %lf %lf"
			// local Earth-fixed NED frame coordinates
			" %lf %lf"
			// position in the localEarth-fixed NED frame
			" %lf %lf %lf",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// latitude, longitude
			&latitude,&longitude,
			// local Earth-fixed NED frame coordinates
			&lat0,&lon0,
			// position in the localEarth-fixed NED frame
			&x,&y,&z
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
//	strcpy(s,str);
}


bool LinearPosition::fread(FILE *f) {
	char str[256];
	// local Earth-fixed NED frame: latitude, longitude
	fscanf(f,"%s %lf",str,&lat0);
    if(strcmp(str,"lat0")!=0)
    {
        printf("LinearPosition::fread lat0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&lon0);
    if(strcmp(str,"lon0")!=0)
    {
        printf("LinearPosition::fread lon0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // position in the localEarth-fixed NED frame
	fscanf(f,"%s %lf",str,&x);
    if(strcmp(str,"x")!=0)
    {
        printf("LinearPosition::fread x expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&y);
    if(strcmp(str,"y")!=0)
    {
        printf("LinearPosition::fread y expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&z);
    if(strcmp(str,"z")!=0)
    {
        printf("LinearPosition::fread z expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // compute (latitude,longitude)
    lat_lon_plus_x_y(latitude,longitude,lat0,lon0,x,y);
	return true;
}

