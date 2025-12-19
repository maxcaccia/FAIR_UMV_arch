/*
 * HydroDynamicsCntrl.cpp
 *
 *  Created on: 2 August 2024
 *      Author: massimo
 */
 
#include "HydroDynamicsCntrl.h"
#include <stdio.h>
 
HydroDynamicsCntrl::HydroDynamicsCntrl(string name):TimeStampedData(name) {
	mPacketCode=HYDRODYNAMICSCNTRL_CODE;
	reset();
}

HydroDynamicsCntrl::HydroDynamicsCntrl(void):TimeStampedData() {
	mPacketCode=HYDRODYNAMICSCNTRL_CODE;
	reset();
}

void HydroDynamicsCntrl::reset(void) {
	mu=mv=0.;
	Ir=0.;
	Xu=Xuu=Yv=Yvv=0.;
	Nr=Nrr=0.;
	mxg=0.;
}

HydroDynamicsCntrl & HydroDynamicsCntrl::operator = (const HydroDynamicsCntrl &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);

	// mass, included added mass
	mu=obj.mu;
	mv=obj.mv;
	// inertia, included added inertia
	Ir=obj.Ir;
	// uncoupled drag coefficients
	Xu=obj.Xu;
	Xuu=obj.Xuu;
	Yv=obj.Yv;
	Yvv=obj.Yvv;
	Nr=obj.Nr;
	Nrr=obj.Nrr;
	// effects of center of mass longitudinal offset
	mxg=obj.mxg;



	// return a reference to myself
	return *this;
}

bool HydroDynamicsCntrl::fread(string fileName) {
	FILE *f;

	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: bool HydroDynamicsCntrl::fread(string fileName) - %s not found!\n",fileName.c_str());
		return false;
	}

	char str[256];
	// mass
	fscanf(f,"%s %lf",str,&mu);
    if(strcmp(str,"mu")!=0)
    {
        printf("HydroDynamicsCntrl::fread mu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&mv);
    if(strcmp(str,"mv")!=0)
    {
        printf("HydroDynamicsCntrl::fread mv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // inertia
	fscanf(f,"%s %lf",str,&Ir);
    if(strcmp(str,"Ir")!=0)
    {
        printf("HydroDynamicsCntrl::fread Ir expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // center of mass longitudinal offset
	fscanf(f,"%s %lf",str,&mxg);
    if(strcmp(str,"mxg")!=0)
    {
        printf("HydroDynamicsCntrl::fread mxg expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
 	// uncoupled drag coefficients
	// Xu, Xuu;
	fscanf(f,"%s %lf",str,&Xu);
    if(strcmp(str,"Xu")!=0)
    {
        printf("HydroDynamicsCntrl::fread Xu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Xuu);
    if(strcmp(str,"Xuu")!=0)
    {
        printf("HydroDynamicsCntrl::fread Xuu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// Yv, Yvv;
	fscanf(f,"%s %lf",str,&Yv);
    if(strcmp(str,"Yv")!=0)
    {
        printf("HydroDynamicsCntrl::fread Yv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Yvv);
    if(strcmp(str,"Yvv")!=0)
    {
        printf("HydroDynamicsCntrl::fread Yvv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// Nr, Nrr;
	fscanf(f,"%s %lf",str,&Nr);
    if(strcmp(str,"Nr")!=0)
    {
        printf("HydroDynamicsCntrl::fread Nr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Nrr);
    if(strcmp(str,"Nrr")!=0)
    {
        printf("HydroDynamicsCntrl::fread Nrr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

	fclose(f);
	return true;

}

int HydroDynamicsCntrl::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// mass/inertia, included added mass/inertia
			"%.2lf %.2lf %.2lf "
			// uncoupled drag coefficients
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			// center of mass longitudinal offset
			"%.3lf "
			" ",
			// time stamp
			str1,
			// mass/inertia, included added mass/inertia
			mu, mv, Ir,
			// uncoupled drag coefficients
			Xu, Xuu,
			Yv, Yvv,
			Nr, Nrr,
			// center of mass longitudinal offset
			mxg
			);
	strcpy(s,str);
	return n;
}

void HydroDynamicsCntrl::sread(char *s) {
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
			// mass/inertia, included added mass/inertia
			" %lf %lf %lf"
			// uncoupled drag coefficients
			" %lf %lf"
			" %lf %lf"
			" %lf %lf"
			// center of mass longitudinal offset
			" %lf "
			"",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// mass/inertia, included added mass/inertia
			&mu, &mv, &Ir,
			// uncoupled drag coefficients
			&Xu, &Xuu,
			&Yv, &Yvv,
			&Nr, &Nrr,
			// center of mass longitudinal offset
			&mxg
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

