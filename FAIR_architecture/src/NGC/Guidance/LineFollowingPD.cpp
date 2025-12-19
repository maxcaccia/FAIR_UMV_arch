/*
 * LineFollowingPD.cpp
 *
 *  Created on: 16 July 2022
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "LineFollowingPD.h"

LineFollowingPD::LineFollowingPD() {
	this->xL=NULL;
	this->yL=NULL;
	this->gammaL=NULL;
	this->xHat=NULL;
	this->yHat=NULL;
	this->psiHat=NULL;
	this->uBar=NULL;
	this->uHat=NULL;
	this->yRef=NULL;
	this->uRef=NULL;

	sigma=0.;
	omega=0.;
	betaBar=0.;
	etaBar=0.;
	rSat=0.;
	uMax=0.;

};

//
// execute RUNNING state task
//
void LineFollowingPD::execute(void)
{
	double xLref=*xL;
	double yLref=*yL;
	double gammaLref=*gammaL;
	double xhat=*xHat;
	double yhat=*yHat;
	double psihat=*psiHat;
	double ubar=*uBar;
	double uhat=*uHat;

	double dx,dy,beta,dBar,d,ddot,bBar;
	double betaRad,gammaRefRad,betaBarRad,betaRef;
	double rref;

	//
	// compute PD gains
	//
	double gP,gD;
	if(uhat!=0.)
	{
		gD=2*sigma/uhat;
		gP=(sigma*sigma+omega*omega)/uhat;
	}
	else
	{
		gP=0.04;
		gD=0.4;
	}
	//
	// compute error
	//
	gammaRefRad=gammaLref*M_PI/180.;
	dx=xLref-xhat;
	dy=yLref-yhat;
	d=dx*sin(gammaRefRad)-dy*cos(gammaRefRad);
	beta=summod180(psihat,-gammaLref);
	betaRad=beta*M_PI/180.;
	//ddot=-xDotHat*sin(gammaRefRad)+yDotHat*cos(gammaRefRad);
	ddot=uhat*sin(betaRad);
	//
	// compute proximity range dBar
	//
	betaBarRad=betaBar*M_PI/180.;
	bBar=sin(betaBarRad);
	dBar=(uhat*bBar-etaBar)*gD/gP;
	if(dBar<0.) dBar=0.;
	//
	// compute guidance action
	//
	if ((beta>=90.)||(beta<=-90.)) // the vehicle is moving opposite to the desired line direction
	{
		if(fabs(d)<=5.) betaRef=0.;
		if(d>5.) betaRef=-betaBar;
		if(d<-5.) betaRef=betaBar;
		if(summod180(beta,-betaRef)>0.) rref=-rSat;
		else rref=rSat;
	}
	else
	{
		rref=-gP*sat(d,-dBar,dBar)-gD*ddot;
		rref=rref*180./M_PI;
		rref=sat(rref,-rSat,rSat);
	}

	*yRef=rref;

	*uRef=sat(ubar,0.,uMax);
}



bool LineFollowingPD::read_configuration(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL)
	{
		printf("ERROR - LineFollowingPD::set_parameters opening file %s\n",filename.c_str());
		return false;
	}

	char str[256];
	fscanf(f,"%s %lf",str,&sigma);
    if(strcmp(str,"sigma")!=0)
    {
        printf("LineFollowingPD::set_parameters sigma expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&omega);
    if(strcmp(str,"omega")!=0)
    {
        printf("LineFollowingPD::set_parameters omega expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&betaBar);
    if(strcmp(str,"betaBar")!=0)
        {
            printf("LineFollowingPD::set_parameters betaBar expected %s found!\n",str);fflush(stdout);
            fclose(f);
            return false;
        }
	fscanf(f,"%s %lf",str,&etaBar);
    if(strcmp(str,"etaBar")!=0)
    {
        printf("LineFollowingPD::set_parameters etaBar expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&rSat);
    if(strcmp(str,"rSat")!=0)
    {
        printf("LineFollowingPD::set_parameters rSat expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&uMax);
    if(strcmp(str,"uMax")!=0)
    {
        printf("LineFollowingPD::set_parameters uMax expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

    fclose(f);

	return true;
}


void LineFollowingPD::set_parameters(double sigma,double omega,double betaBar,double etaBar,double rSat,double uMax) {
	this->sigma = sigma;
	this->omega = omega;
	this->betaBar = betaBar;
	this->etaBar = etaBar;
	this->rSat = rSat;
	this->uMax = uMax;
}


void LineFollowingPD::set_input_output(double *xL,double *yL,double *gammaL,double *xHat,double *yHat,double *psiHat,double *uBar,double *uHat,double *yRef,double *uRef) {
	this->xL=xL;
	this->yL=yL;
	this->gammaL=gammaL;
	this->xHat=xHat;
	this->yHat=yHat;
	this->psiHat=psiHat;
	this->uBar=uBar;
	this->uHat=uHat;
	this->yRef=yRef;
	this->uRef=uRef;

};


void LineFollowingPD::print(void) {
	printf("LineFollowingPD::print %s\n",mName.c_str()); fflush(stdout);
	GuidanceControlAlgo::print();

	printf("controller parameters:\n");
	printf("sigma : %.3lf\nomega : %.3lf\nbetaBar : %.3lf\netaBar : %.3lf\nrSat : %.3lf\nuMax : %.3lf\n",
			sigma,omega,betaBar,etaBar,rSat,uMax);

}



