/*
 * LineOfSight.cpp
 *
 *  Created on: 11 giu 2022
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "LineOfSight.h"
#include "nVector.h"

LineOfSight::LineOfSight(void): GuidanceControlAlgo() {
	this->latRef = NULL;
	this->lonRef = NULL;
	this->latHat = NULL;
	this->lonHat = NULL;
	this->uBar=NULL;
	this->psiRef = NULL;
	this->uRef = NULL;

	uMax=0.;
	roWP=0.;
}


void LineOfSight::execute(void) {
	double psiRad,dx,dy;

	//
	// the position of the target way-point is computed with respect to a NED frame positioned at the vehicle coordinates
	//
	lat_lon_2_x_y(dx,dy,*latHat,*lonHat,*latRef,*lonRef);

	psiRad = atan2(dy,dx);

	*psiRef = psiRad*180./M_PI;
	
	double d=dx*dx+dy*dy;
	if(d>roWP) {
		*uRef=sat(*uBar,0.,uMax);
	}
	else {
		*uRef=d*sat(*uBar,0.,uMax)/roWP;
	}

	printf("LineOfSight::execute dx %lf dy %lf psiRef %lf\n",dx,dy,*psiRef);
}


bool LineOfSight::read_configuration(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL)
	{
		printf("ERROR - LineOfSight::set_parameters opening file %s\n",filename.c_str());
		return false;
	}

	char str[256];
	fscanf(f,"%s %lf",str,&uMax);
    if(strcmp(str,"uMax")!=0)
    {
        printf("LineOfSight::set_parameters uMax expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	fscanf(f,"%s %lf",str,&roWP);
    if(strcmp(str,"roWP")!=0)
    {
        printf("LineOfSight::set_parameters roWP expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

    fclose(f);

	return true;
}



void LineOfSight::set_input_output(double *latRef,double *lonRef,double *latHat,double *lonHat,double *uBar,double *psiRef,double *uRef) {
	this->latRef = latRef;
	this->lonRef = lonRef;
	this->latHat = latHat;
	this->lonHat = lonHat;
	this->uBar=uBar;
	this->psiRef = psiRef;
	this->uRef = uRef;
}


void LineOfSight::print(void) {
	printf("LineOfSight::print %s\n",mName.c_str()); fflush(stdout);
	GuidanceControlAlgo::print();

	printf("controller parameters:\n");
	printf("uMax : %.3lf\nroWP : %.3lf\n",
			uMax,roWP);

}

