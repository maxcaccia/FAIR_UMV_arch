/*
 * HydroDynamics.cpp
 *
 *  Created on: 16 November 2022
 *      Author: massimo
 */
 
#include "HydroDynamics.h"
#include <stdio.h>
 
HydroDynamics::HydroDynamics(string name):TimeStampedData(name) {
	mPacketCode=HYDRODYNAMICS_CODE;
	reset();
}

HydroDynamics::HydroDynamics(void):TimeStampedData() {
	mPacketCode=HYDRODYNAMICS_CODE;
	reset();
}

void HydroDynamics::reset(void) {
	m=0.;
	XuDot=YvDot=0.;
	Iz=0.;
	NrDot=0.;
	Xu=Xuu=Yv=Yvv=0.;
	Nr=Nrr=0.;
	xg=0.;

	kr2=kur=kXu=0.;

	mu=mv=0.;
	Ir=0.;
}

HydroDynamics & HydroDynamics::operator = (const HydroDynamics &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);

	// mass and added mass
	m=obj.m;
	XuDot=obj.XuDot;
	YvDot=obj.YvDot;
	// inertia and included added inertia
	Iz=obj.Iz;
	NrDot=obj.NrDot;
	// uncoupled drag coefficients
	Xu=obj.Xu;
	Xuu=obj.Xuu;
	Yv=obj.Yv;
	Yvv=obj.Yvv;
	Nr=obj.Nr;
	Nrr=obj.Nrr;
	// xg : center of mass longitudinal offset
	xg=obj.xg;

	// kr2 : square(yawrate) induced surge force coefficient
	kr2=obj.kr2;
	// kur : surge-yawrate interaction coefficient
	kur=obj.kur;
	// surge force reduction induced by speed coefficient
	kXu=obj.kXu;;

	// mass, included added mass
	mu=obj.mu;
	mv=obj.mv;
	// inertia, included added inertia
	Ir=obj.Ir;

	// return a reference to myself
	return *this;
}

bool HydroDynamics::fread(string fileName) {
	FILE *f;

	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: bool HydroDynamics::fread(string fileName) - %s not found!\n",fileName.c_str());
		return false;
	}

	char str[256];
	// mass
	fscanf(f,"%s %lf",str,&m);
    if(strcmp(str,"m")!=0)
    {
        printf("HydroDynamics::fread m expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // inertia
	fscanf(f,"%s %lf",str,&Iz);
    if(strcmp(str,"Iz")!=0)
    {
        printf("HydroDynamics::fread Iz expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // added mass
	fscanf(f,"%s %lf",str,&XuDot);
    if(strcmp(str,"XuDot")!=0)
    {
        printf("HydroDynamics::fread XuDot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    mu = m - XuDot;
	fscanf(f,"%s %lf",str,&YvDot);
    if(strcmp(str,"YvDot")!=0)
    {
        printf("HydroDynamics::fread YvDot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    mv = m - YvDot;
    // added inertia
	fscanf(f,"%s %lf",str,&NrDot);
    if(strcmp(str,"NrDot")!=0)
    {
        printf("HydroDynamics::fread NrDot expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    Ir = Iz - NrDot;
    // center of mass longitudinal offset
	fscanf(f,"%s %lf",str,&xg);
    if(strcmp(str,"xg")!=0)
    {
        printf("HydroDynamics::fread xg expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
 	// uncoupled drag coefficients
	// Xu, Xuu;
	fscanf(f,"%s %lf",str,&Xu);
    if(strcmp(str,"Xu")!=0)
    {
        printf("HydroDynamics::fread Xu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Xuu);
    if(strcmp(str,"Xuu")!=0)
    {
        printf("HydroDynamics::fread Xuu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// Yv, Yvv;
	fscanf(f,"%s %lf",str,&Yv);
    if(strcmp(str,"Yv")!=0)
    {
        printf("HydroDynamics::fread Yv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Yvv);
    if(strcmp(str,"Yvv")!=0)
    {
        printf("HydroDynamics::fread Yvv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// Nr, Nrr;
	fscanf(f,"%s %lf",str,&Nr);
    if(strcmp(str,"Nr")!=0)
    {
        printf("HydroDynamics::fread Nr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Nrr);
    if(strcmp(str,"Nrr")!=0)
    {
        printf("HydroDynamics::fread Nrr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // kr2 : square(yawrate) induced surge force coefficient
	fscanf(f,"%s %lf",str,&kr2);
    if(strcmp(str,"kr2")!=0)
    {
        printf("HydroDynamics::fread kr2 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// kur : surge-yawrate interaction coefficient
	fscanf(f,"%s %lf",str,&kur);
    if(strcmp(str,"kur")!=0)
    {
        printf("HydroDynamics::fread kur expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// kXu : surge force reduction induced by speed coefficient
	fscanf(f,"%s %lf",str,&kXu);
    if(strcmp(str,"kXu")!=0)
    {
        printf("HydroDynamics::fread kXu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }


	fclose(f);
	return true;

}

int HydroDynamics::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// mass and added mass
			"%.2lf %.2lf %.2lf "
			// inertia and added inertia
			"%.2lf %.2lf "
			// uncoupled drag coefficients
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			// center of mass longitudinal offset
			"%.3lf "
			// kr2 : square(yawrate) induced surge force coefficient
			"%.3lf "
			// kur : surge-yawrate interaction coefficient
			"%.3lf "
			// kXu : surge force reduction induced by speed coefficient
			"%.3lf "
			// mass/inertia, included added mass/inertia
			"%.2lf %.2lf %.2lf "
			" ",
			// time stamp
			str1,
			// mass and added mass
			m,XuDot,YvDot, // [Kg]
			// inertia and added inertia
			Iz,NrDot, // [Kg m]
			// uncoupled drag coefficients
			Xu, Xuu,
			Yv, Yvv,
			Nr, Nrr,
			// center of mass longitudinal offset
			xg,
			// kr2 : square(yawrate) induced surge force coefficient
			kr2,
			// kur : surge-yawrate interaction coefficient
			kur,
			// kXu : surge force reduction induced by speed coefficient
			kXu,
			// mass/inertia, included added mass/inertia
			mu, mv, Ir
			);
	strcpy(s,str);
	return n;
}

void HydroDynamics::sread(char *s) {
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
			// mass and added mass
			" %lf %lf %lf "
			// inertia and added inertia
			" %lf %lf "
			// uncoupled drag coefficients
			" %lf %lf"
			" %lf %lf"
			" %lf %lf"
			// center of mass longitudinal offset
			" %lf "
			// kr2 : square(yawrate) induced surge force coefficient
			" %lf "
			// kur : surge-yawrate interaction coefficient
			" %lf "
			// kXu : surge force reduction induced by speed coefficient
			" %lf "
			// mass/inertia, included added mass/inertia
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
			// mass and added mass
			&m,&XuDot,&YvDot, // [Kg]
			// inertia and added inertia
			&Iz,&NrDot, // [Kg m]
			// uncoupled drag coefficients
			&Xu, &Xuu,
			&Yv, &Yvv,
			&Nr, &Nrr,
			// center of mass longitudinal offset
			&xg,
			// kr2 : square(yawrate) induced surge force coefficient
			&kr2,
			// kur : surge-yawrate interaction coefficient
			&kur,
			// kXu : surge force reduction induced by speed coefficient
			&kXu,
			// mass/inertia, included added mass/inertia
			&mu, &mv, &Ir
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

