/*
 * GainSchedPI.cpp
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "GainSchedPI.h"


GainSchedPI::GainSchedPI(double dt): GuidanceControlAlgo() {
	this->xRef=NULL;
	this->xHat=NULL;
	this->yRef=NULL;
	this->mx=0.;
	this->kx=0.;
	this->kxx=0.;
	mDt=dt;
//	mDegreeFlag=flag;

	reset();
};


void GainSchedPI::execute(void)
{
	double kP,kI,satIe,e,uBar,uDelta;

	double xref=*xRef;
	double xhat=*xHat;
	double yref;
//	double XhatRef;
//	if(mDegreeFlag)
//		XhatRef=*Xhat;
//	else
//		XhatRef=0.;

	e=xref-xhat;
//	if(mDegreeFlag)
//		e*=M_PI/180.;

	double a;
//	if(mDegreeFlag) {
//		uBar=-kx*(xref*M_PI/180.)-kxx*(xref*M_PI/180.)*fabs((xref*M_PI/180.))-kXx*(xref*M_PI/180.)*sqrt(fabs(XhatRef));
//		a=(-kx-kXx*sqrt(fabs(XhatRef))-2.*kxx*fabs(xref*M_PI/180.))/mx;
//	}
//	else {
//		uBar=-kx*(xref*M_PI/180.)-kxx*(xref*M_PI/180.)*fabs((xref*M_PI/180.));
//		a=(-kx-2.*kxx*fabs(xref*M_PI/180.))/mx;
//	}
	uBar=compute_feedforward_action(xref);
	a=(kx+2.*kxx*fabs(xref))/mx;
	kP=mx*(2.*mSigma-a);
	kI=mx*(SQR(mSigma)+SQR(mOmega));

	//
	// how to compute the saturation value for the integral action?
	//
	satIe=(compute_feedforward_action(fabs(xref)+fabs(deltaXmax))-compute_feedforward_action(fabs(xref)))/kI;
//	satIe=((fabs(kx)+fabs(kXx)*sqrt(fabs(XhatRef)))*(x0MaxNoise*M_PI/180.)+fabs(kxx)*SQR((x0MaxNoise*M_PI/180.)))/kI;

	Ie+=e*mDt;
	if(Ie>satIe) Ie=satIe;
	else if(Ie<-satIe) Ie=-satIe;

	uDelta=kP*e+kI*Ie;

	yref=uDelta+uBar;
	yref=sat(yref,ySatMin,ySatMax);

	*yRef=yref;
}


void GainSchedPI::reset(void)
{
	Ie=0.;
}


bool GainSchedPI::read_configuration(string filename) {

	FILE *f;
//	char fileName[128];
//	sprintf(fileName,"%s",filename.c_str());

	if((f=fopen(filename.c_str(),"r"))==NULL)
	{
		printf("ERROR - GainSchedPI::set_parameters opening file %s\n",filename.c_str());fflush(stdout);
		return false;
	}

	char str[256];
	fscanf(f,"%s %lf",str,&mSigma);
    if(strcmp(str,"sigma")!=0)
    {
        printf("GainSchedPI::set_parameters sigma expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&mOmega);
    if(strcmp(str,"omega")!=0)
    {
        printf("GainSchedPI::set_parameters omega expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&deltaXmax);
    if(strcmp(str,"deltaXmax")!=0)
    {
        printf("GainSchedPI::set_parameters deltaXmax expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&ySatMin);
    if(strcmp(str,"ySatMin")!=0)
    {
    	printf("GainSchedPI::set_parameters ySatMin expected %s found!\n",str);fflush(stdout);
    	fclose(f);
    	return false;
        }
        
	fscanf(f,"%s %lf",str,&ySatMax);
    if(strcmp(str,"ySatMax")!=0)
    {
        printf("GainSchedPI::set_parameters ySatMax expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

    fclose(f);

printf("GainSchedPI::read_configuration %s : configuration read\n",filename.c_str());fflush(stdout);
	return true;
}


void GainSchedPI::set_input_output(double *xRef,double *xHat,double *yRef) {
	printf("void GainSchedPI::set_input_output(double *xRef,double *xHat,double *yRef) %s\n",mName.c_str());
	this->xRef=xRef;
	this->xHat=xHat;
	this->yRef=yRef;

	reset();
};



void GainSchedPI::set_parameters(double sigma,double omega,double deltaXmax,double ySat) {
	this->mSigma=sigma;
	this->mOmega=omega;
	this->deltaXmax=deltaXmax;
	this->ySatMax = ySat;
	this->ySatMin = -ySat;
}


void GainSchedPI::set_hydrodynamics_parameters(double mx,double kx,double kxx) {
	this->mx=mx;
	this->kx=kx;
	this->kxx=kxx;
}


void GainSchedPI::print(void) {
	printf("GainSchedPI::print %s\n",mName.c_str()); fflush(stdout);
	GuidanceControlAlgo::print();
	printf("hydrodynamics parameters:\n");
	printf("mx : %.3lf\nkx : %.3lf\nkxx : %.3lf\n",mx,kx,kxx); fflush(stdout);
	printf("controller parameters:\n");
	printf("sigma : %.3lf\nomega : %.3lf\ndeltaXmax : %.3lf\nySatMin : %.3lf\nySatMax : %.3lf\n",
			mSigma,mOmega,deltaXmax,ySatMin,ySatMax);
}


GainSchedPIyaw::GainSchedPIyaw(double dt): GainSchedPI(dt) {
	this->uHat=NULL;
	this->kux=0.;
};


void GainSchedPIyaw::execute(void)
{
	double kP,kI,satIe,e,uBar,uDelta;

	double xref=*xRef;
	double xhat=*xHat;
	double uhat=*uHat;
	double yref;


	e=xref-xhat;
	e*=M_PI/180.;

	double a;
	uBar=compute_feedforward_action(xref*M_PI/180.,uhat);
	a=(kx+2.*kxx*fabs(xref*M_PI/180.)+kux*uhat)/mx;
	kP=mx*(2.*mSigma-a);
	kI=mx*(SQR(mSigma)+SQR(mOmega));

	//
	// how to compute the saturation value for the integral action?
	//
	satIe=(compute_feedforward_action(fabs(xref)*M_PI/180.+fabs(deltaXmax*M_PI/180.),uhat)-compute_feedforward_action(fabs(xref)*M_PI/180.,uhat))/kI;

	Ie+=e*mDt;
	if(Ie>satIe) Ie=satIe;
	else if(Ie<-satIe) Ie=-satIe;

	uDelta=kP*e+kI*Ie;

	yref=uDelta+uBar;
	yref=sat(yref,ySatMin,ySatMax);

	*yRef=yref;
}


void GainSchedPIyaw::set_input_output(double *xRef,double *xHat,double *yRef,double *uHat) {
	printf("void GainSchedPIyaw::set_input_output(double *xRef,double *xHat,double *yRef) %s\n",mName.c_str());
	this->xRef=xRef;
	this->xHat=xHat;
	this->yRef=yRef;
	this->uHat=uHat;

	reset();
};

void GainSchedPIyaw::set_hydrodynamics_parameters(double mx,double kx,double kxx,double kux) {
	GainSchedPI::set_hydrodynamics_parameters(mx,kx,kxx);
	this->kux=kux;
}


