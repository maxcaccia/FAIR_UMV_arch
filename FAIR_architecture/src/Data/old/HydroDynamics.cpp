/*
 * HydroDynamics.cpp
 *
 *  Created on: 16 November 2022
 *      Author: massimo
 */
 
#include "HydroDynamics.h"
#include <stdio.h>
 
HydroDynamics::HydroDynamics(string name):TimeStampedData(name) {
	reset();
}

HydroDynamics::HydroDynamics(void):TimeStampedData() {
	reset();
}

void HydroDynamics::reset(void) {
	mu=mv=mw=0;
	Ip=Iq=Ir=0;
	ku=kuu=kv=kvv=kw=kww=0;
	kp=kpp=kq=kqq=kr=krr=0;
	kvr=kur=kru=0;
	krX=0;
}

HydroDynamics & HydroDynamics::operator = (const HydroDynamics &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// mass, included added mass
	mu=obj.mu;
	mv=obj.mv;
	mw=obj.mw;
	// inertia, included added inertia
	Ip=obj.Ip;
	Iq=obj.Iq;
	Ir=obj.Ir;
	// uncoupled drag coefficients
	ku=obj.ku;
	kuu=obj.kuu;
	kv=obj.kv;
	kvv=obj.kvv;
	kw=obj.kw;
	kww=obj.kww;
	kp=obj.kp;
	kpp=obj.kpp;
	kq=obj.kq;
	kqq=obj.kqq;
	kr=obj.kr;
	krr=obj.krr;
	// coupled drag coefficients
	kvr=obj.kvr;
	kur=obj.kur;
	kru=obj.kru;
	// coupled force angular rate coefficients
	krX=obj.krX;
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
	// mass, included added mass
	fscanf(f,"%s %lf",str,&mu);
    if(strcmp(str,"mu")!=0)
    {
        printf("HydroDynamics::fread mu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&mv);
    if(strcmp(str,"mv")!=0)
    {
        printf("HydroDynamics::fread mv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&mw);
    if(strcmp(str,"mw")!=0)
    {
        printf("HydroDynamics::fread mw expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

	// inertia, included added inertia
	fscanf(f,"%s %lf",str,&Ip);
    if(strcmp(str,"Ip")!=0)
    {
        printf("HydroDynamics::fread Ip expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Iq);
    if(strcmp(str,"Iq")!=0)
    {
        printf("HydroDynamics::fread Iq expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Ir);
    if(strcmp(str,"Ir")!=0)
    {
        printf("HydroDynamics::fread Ir expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// uncoupled drag coefficients
	// ku, kuu;
	fscanf(f,"%s %lf",str,&ku);
    if(strcmp(str,"ku")!=0)
    {
        printf("HydroDynamics::fread ku expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&kuu);
    if(strcmp(str,"kuu")!=0)
    {
        printf("HydroDynamics::fread kuu expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// kv, kvv;
	fscanf(f,"%s %lf",str,&kv);
    if(strcmp(str,"kv")!=0)
    {
        printf("HydroDynamics::fread kv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&kvv);
    if(strcmp(str,"kvv")!=0)
    {
        printf("HydroDynamics::fread kvv expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// kw, kww;
	fscanf(f,"%s %lf",str,&kw);
    if(strcmp(str,"kw")!=0)
    {
        printf("HydroDynamics::fread kw expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&kww);
    if(strcmp(str,"kww")!=0)
    {
        printf("HydroDynamics::fread kww expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// kp, kpp;
	fscanf(f,"%s %lf",str,&kp);
    if(strcmp(str,"kp")!=0)
    {
        printf("HydroDynamics::fread kp expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&kpp);
    if(strcmp(str,"kpp")!=0)
    {
        printf("HydroDynamics::fread kpp expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// kq, kqq;
	fscanf(f,"%s %lf",str,&kq);
    if(strcmp(str,"kq")!=0)
    {
        printf("HydroDynamics::fread kq expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&kqq);
    if(strcmp(str,"kqq")!=0)
    {
        printf("HydroDynamics::fread kqq expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// kr, krr;
	fscanf(f,"%s %lf",str,&kr);
    if(strcmp(str,"kr")!=0)
    {
        printf("HydroDynamics::fread kr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&krr);
    if(strcmp(str,"krr")!=0)
    {
        printf("HydroDynamics::fread krr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// coupled drag coefficients
	// kvr,kur,kru;
	fscanf(f,"%s %lf",str,&kvr);
    if(strcmp(str,"kvr")!=0)
    {
        printf("HydroDynamics::fread kvr expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&kur);
    if(strcmp(str,"kur")!=0)
    {
        printf("HydroDynamics::fread kur expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&kru);
    if(strcmp(str,"kru")!=0)
    {
        printf("HydroDynamics::fread kru expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// coupled force angular rate coefficients
	fscanf(f,"%s %lf",str,&krX);
    if(strcmp(str,"krX")!=0)
    {
        printf("HydroDynamics::fread krX expected %s found!\n",str);fflush(stdout);
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
	n=sprintf(str,"HYDRO_DYN %s"
			// mass, included added mass
			"%.2lf %.2lf %.2lf "
			// inertia, included added inertia
			"%.2lf %.2lf %.2lf "
			// uncoupled drag coefficients
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			"%.3lf %.3lf "
			// coupled drag coefficients
			"%.3lf %.3lf %.3lf "
			// coupled force angular rate coefficients
			"%.3lf"
			" ",
			// time stamp
			str1,
			// mass, included added mass
			mu,mv,mw, // [Kg]
			// inertia, included added inertia
			Ip,Iq,Ir, // [Kg m]
			// uncoupled drag coefficients
			ku, kuu,
			kv, kvv,
			kw, kww,
			kp, kpp,
			kq, kqq,
			kr, krr,
			// coupled drag coefficients
			kvr, kur,kru,
			// coupled force angular rate coefficients
			krX
			);
	strcpy(s,str);
	return n;
}

void HydroDynamics::sread(char *s) {
			char str[64];
			char name[64];
			sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			// mass, included added mass
			" %lf %lf %lf"
			// inertia, included added inertia
			" %lf %lf %lf"
			// uncoupled drag coefficients
			" %lf %lf"
			" %lf %lf"
			" %lf %lf"
			" %lf %lf"
			" %lf %lf"
			" %lf %lf"
			// coupled drag coefficients
			" %lf %lf %lf"
			// coupled force angular rate coefficients
			" %lf"
			"",
			str,
			// time stamp
			name,&mTimeStamp,
			// mass, included added mass
			&mu,&mv,&mw, // [Kg]
			// inertia, included added inertia
			&Ip,&Iq,&Ir, // [Kg m]
			// uncoupled drag coefficients
			&ku, &kuu,
			&kv, &kvv,
			&kw, &kww,
			&kp, &kpp,
			&kq, &kqq,
			&kr, &krr,
			// coupled drag coefficients
			&kvr, &kur,&kru,
			// coupled force angular rate coefficients
			&krX
			);
			mName=name;
}

