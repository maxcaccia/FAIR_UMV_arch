/*
 * MathAngles.cpp
 *
 *  Created on: Oct 24, 2023
 *      Author: mc
 */

#include "MathAngles.h"

//
// modpi assumes x in (-pi-2*pi,pi+2*pi]
// - it is basically applied to angles in the interval [-2*pi,2*pi]

//
double modpi(double x)
{
	double y=x;
	if(y>M_PI) y=y-2.0*M_PI;
	if(y<=-M_PI) y=y+2.0*M_PI;
	return y;
}

//
// mod180 assumes x in (-540,540]
// - it is basically applied to angles in the interval [-360,360]
//
double mod180(double x)
{
	double y=x;
	if(y>180.0) y=y-360.0;
	if(y<=-180.0) y=y+360.0;
	return y;
}

//
// mod360 assumes x in (-360,720]
// - it is basically applied to angles in the interval [-180,180]
//
double mod360(double x)
{
	double y=x;
    if(y>=360.0) y=y-360.0;
    if(y<0.0) y=y+360.0;
    return y;
}

double summodpi(double alfa,double beta)
{
double teta=modpi(alfa)+modpi(beta);
teta=modpi(teta);
//if(teta>M_PI) teta=teta-2.*M_PI;
//else if(teta<=-M_PI) teta=teta+2.*M_PI;
return(teta);
}

double summod180(double alfa,double beta)
{
double teta=mod180(alfa)+mod180(beta);
teta=mod180(teta);
//if(teta>180.) teta=teta-360.;
//else if(teta<=-180.) teta=teta+360.;
return(teta);
}

double summod360(double alfa,double beta)
{
double teta=mod180(alfa)+mod180(beta);
teta=mod360(teta);
//if(teta>=360.) teta=teta-360.;
//else if(teta<0.) teta=teta+360.;
return(teta);
}

double summod90(double alfa,double beta)
{
	double teta=alfa+beta;
	if(teta>90.0) teta=180.-teta; //90.-(teta-90.);
	else if(teta<-90.0) teta=-180.-teta;
	return teta;
}


void unwrap(double x,double &xRef)
{
	double dx=module360(x);
	double y0=360.*floor(xRef/360.);
	double xPred=y0+dx;
	double xNew;
	double delta=xRef-xPred;
	if(delta>=180.)
		xNew=xPred+360;
	else if(delta<-180.)
		xNew=xPred-360.;
	else
		xNew=xPred;
	xRef=xNew;
//	double xNew=x;
//	double delta;
//
//	delta=xRef-xNew;
//	while(fabs(delta)>180)
//	{
//		if(delta>180.)
//			xNew+=360.;
//		else
//			xNew-=360;
//		delta=xRef-xNew;
//	}
//	xRef=xNew;
}

//
// wrap the value of x in (-180,180]
//
double wrap180(double x)
{
	double y;
	y=mod180(module360(x));
	return y;
	//double y=x;
	//while(fabs(y)>180.)
		//if(y>180.) y-=360.;
		//else y+=360.;
	//if(y==-180.) y=180.;
	//return y;
}

double module(double x,double y) {
	double m=floor(x/y);
	return (x-m*y);
}

double module360(double x) {
	return module(x,360.);
}



