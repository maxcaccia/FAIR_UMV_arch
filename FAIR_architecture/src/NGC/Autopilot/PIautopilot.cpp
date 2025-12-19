/*
 * PIsiso.cpp
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "PIautopilot.h"

PIautopilot::PIautopilot(double dt) {
	this->xRef=NULL;
	this->xHat=NULL;
	this->uBar=NULL;
	this->yRef=NULL;
	this->uRef=NULL;

	mDt=dt;

	eI=0.;
	flagI=false;
	kP=0.;
	gI=0.;
	Isat=0.;
	eIon=0.;
	eIoff=0.;
	ySat=0.;
	uMax=0.;

	reset();
};

//
// execute RUNNING state task
//
void PIautopilot::execute(void)
{
	double e,eabs;

	double xref=*xRef;
	double xhat=*xHat;
	double ubar=*uBar;

	e=summod180(xref,-xhat);

	eabs=fabs(e);
	if(eabs>eIoff) flagI=false;
	else if(eabs<eIon) flagI=true;

	if(flagI==false) eI=0.;
	else eI=sat(eI+e*mDt,-Isat,Isat);

	double yref=kP*(e+gI*eI);
	yref=sat(yref,-ySat,ySat);

	*yRef=yref;
	
	*uRef=sat(ubar,0.,uMax);

	//printf("PIautopilot::execute xref %lf xhat %lf e %lf yref %lf\n",xref,xhat,e,yref);
}


void PIautopilot::reset(void)
{
 eI=0.;
 flagI=false;
}


bool PIautopilot::read_configuration(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL)
	{
		printf("ERROR - PIautopilot::set_parameters opening file %s\n",filename.c_str());
		return false;
	}

	char str[256];
	fscanf(f,"%s %lf",str,&kP);
    if(strcmp(str,"kP")!=0)
    {
        printf("PIautopilot::set_parameters kP expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&gI);
    if(strcmp(str,"gI")!=0)
    {
        printf("PIautopilot::set_parameters gI expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&Isat);
    if(strcmp(str,"Isat")!=0)
        {
            printf("PIautopilot::set_parameters Isat expected %s found!\n",str);fflush(stdout);
            fclose(f);
            return false;
        }
	fscanf(f,"%s %lf",str,&eIon);
    if(strcmp(str,"eIon")!=0)
    {
        printf("PIautopilot::set_parameters eIon expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&eIoff);
    if(strcmp(str,"eIoff")!=0)
    {
        printf("PIautopilot::set_parameters eIoff expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&ySat);
    if(strcmp(str,"ySat")!=0)
    {
        printf("PIautopilot::set_parameters ySat expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&uMax);
    if(strcmp(str,"uMax")!=0)
    {
        printf("PIautopilot::set_parameters uMax expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

    fclose(f);

	return true;
}


void PIautopilot::set_parameters(double kP,double gI,double Isat,double eIon,double eIoff,double ySat,double uMax) {
	this->kP = kP;
	this->gI = gI;
	this->Isat = Isat;
	this->eIon = eIon;
	this->eIoff = eIoff;
	this->ySat = ySat;
	this->uMax = uMax;

	reset();
}

void PIautopilot::set_input_output(double *xRef,double *xHat,double *uBar,double *yRef,double *uRef) {
	printf("void PIautopilot::set_input_output(double *xRef,double *xHat,double *yRef) %s\n",mName.c_str());
	this->xRef=xRef;
	this->xHat=xHat;
	this->uBar=uBar;
	this->yRef=yRef;
	this->uRef=uRef;

	reset();
};


void PIautopilot::print(void) {
	printf("PIautopilot::print %s\n",mName.c_str()); fflush(stdout);
	GuidanceControlAlgo::print();

	printf("controller parameters:\n");
	printf("kP : %.3lf\ngI : %.3lf\nIsat : %.3lf\neIon : %.3lf\neIoff : %.3lf\nySat : %.3lf\nuMax : %.3lf\n",
			kP,gI,Isat,eIon,eIoff,ySat,uMax);

}




