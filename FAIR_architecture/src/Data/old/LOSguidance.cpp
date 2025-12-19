/*
 * LOSguidance.cpp
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
 
#include "LOSguidance.h"
#include <stdio.h>
 
LOSguidance::LOSguidance(string name):TimeStampedData(name) {
	reset();
}

LOSguidance::LOSguidance(void):TimeStampedData() {
	reset();
}


void LOSguidance::reset(void) {
	latLOS=0;
	lonLOS=0;
	lat0=0;
	lon0=0;
	xLOS=0.;
	yLOS=0.;
	uLOS=0.;
}

LOSguidance & LOSguidance::operator = (const LOSguidance &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// latitude, longitude of a line point
	latLOS=obj.latLOS;
	lonLOS=obj.lonLOS;
	// local Earth-fixed NED frame coordinates
	lat0=obj.lat0;
	lon0=obj.lon0;
	// position of a line point in the local Earth-fixed NED frame
	xLOS=obj.xLOS;
	yLOS=obj.yLOS;
	// linear velocity in vehicle-fixed frame
	uLOS=obj.uLOS;
	// return a reference to myself
	return *this;
}

void LOSguidance::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"%s_latitude,"
			"%s_longitude,"
			"%s_latitude0,"
			"%s_longitude0,"
			"%s_x,"
			"%s_y,"
			"%s_surge_rate_fore,"
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


void LOSguidance::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"latitude,"								//CF
			"longitude,"							//CF
			"latitude,"								//CF
			"longitude,"							//CF
			"projection_x_coordinate,"				//CF
			"projection_y_coordinate,"				//CF
			"platform_surge_rate_fore,"				//CF
			,strTS
			);
	strcpy(s,str);
}


int LOSguidance::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"LOS %s"
			// angles
			"%.8lf %.8lf "
			// local Earth-fixed NED frame coordinates
			"%.8lf %.8lf "
			// position in the localEarth-fixed NED frame
			"%.2lf %.2lf "
			// linear velocity in vehicle-fixed frame
			"%.2lf "
			" ",
			// time stamp
			str1,
			// latitude, longitude
			latLOS,lonLOS,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			xLOS,yLOS,
			// linear velocity in vehicle-fixed frame
			uLOS
			);
	strcpy(s,str);
	return n;
}


void LOSguidance::sprint_csv(char *s) {
	char str[1024],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"LOS,%s"
			// latitude, longitude
			"%.8lf,%.8lf,"
			// local Earth-fixed NED frame coordinates
			"%.8lf,%.8lf,"
			// position in the localEarth-fixed NED frame
			"%.2lf,%.2lf,"
			// linear velocity in vehicle-fixed frame
			"%.2lf"
			",",
			// time stamp
			str1,
			// latitude, longitude
			latLOS,lonLOS,
			// local Earth-fixed NED frame coordinates
			lat0,lon0,
			// position in the localEarth-fixed NED frame
			xLOS,yLOS,
			// linear velocity in vehicle-fixed frame
			uLOS
			);
	strcpy(s,str);
}

void LOSguidance::sread(char *s) {
	char str[64];
	char name[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			// latitude, longitude
			" %lf %lf"
			// local Earth-fixed NED frame coordinates
			" %lf %lf"
			// position in the localEarth-fixed NED frame
			" %lf %lf"
			// linear velocity in vehicle-fixed frame
			" %lf",
			str,
			// time stamp
			name,&mTimeStamp,
			// latitude, longitude
			&latLOS,&lonLOS,
			// local Earth-fixed NED frame coordinates
			&lat0,&lon0,
			// position in the localEarth-fixed NED frame
			&xLOS,&yLOS,
			// linear velocity in vehicle-fixed frame
			&uLOS // surge [m/s]
			);
	mName=name;
//	strcpy(s,str);
}


bool LOSguidance::fread(string fileName) {
	FILE *f;

	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: bool LOSguidance::fread(string fileName) - %s not found!\n",fileName.c_str());
		return false;
	}

	char str[256];
	// local Earth-fixed NED frame: latitude, longitude
	fscanf(f,"%s %lf",str,&lat0);
    if(strcmp(str,"lat0")!=0)
    {
        printf("LOSguidance::fread lat0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&lon0);
    if(strcmp(str,"lon0")!=0)
    {
        printf("LOSguidance::fread lon0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // position in the localEarth-fixed NED frame
	fscanf(f,"%s %lf",str,&xLOS);
    if(strcmp(str,"xLOS")!=0)
    {
        printf("LOSguidance::fread xLOS expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&yLOS);
    if(strcmp(str,"yLOS")!=0)
    {
        printf("LOSguidance::fread yLOS expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // compute (latitude,longitude)
    lat_lon_plus_x_y(latLOS,lonLOS,lat0,lon0,xLOS,yLOS);
    // linear velocity in vehicle-fixed frame
	fscanf(f,"%s %lf",str,&uLOS);
    if(strcmp(str,"uLOS")!=0)
    {
        printf("LOSguidance::fread uLOS expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

	fclose(f);
	return true;

}




