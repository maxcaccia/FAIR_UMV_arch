/*
 * MathAngles.cpp
 *
 *  Created on: Oct 24, 2023
 *      Author: mc
 */

#include "MathAngles.h"

double modpi(double x)
{
	double y=x;
	while (y>M_PI)
		y=y-2.0*M_PI;
	while (y<=-M_PI)
	    y=y+2.0*M_PI;
	return y;
}

double mod180(double x)
{
	double y=x;
	while (y>180.0) y=y-360.0;
	while (y<=-180.0) y=y+360.0;
	return y;
}

double mod360(double x)
{
    while(x>=360.0) x-=360.0;
    while(x<0.0) x+=360.0;
    return x;
}

double summodpi(double alfa,double beta)
{
double teta=modpi(alfa)+modpi(beta);
if(teta>M_PI) teta=teta-2.*M_PI;
else if(teta<=-M_PI) teta=teta+2.*M_PI;
return(teta);
}

double summod180(double alfa,double beta)
{
double teta=mod180(alfa)+mod180(beta);
if(teta>180.) teta=teta-360.;
else if(teta<=-180.) teta=teta+360.;
return(teta);
}

double summod360(double alfa,double beta)
{
double teta=mod180(alfa)+mod180(beta);
if(teta>=360.) teta=teta-360.;
else if(teta<0.) teta=teta+360.;
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
	double xNew=x;
	double delta;

	delta=xRef-xNew;
	while(fabs(delta)>180)
	{
		if(delta>180.)
			xNew+=360.;
		else
			xNew-=360;
		delta=xRef-xNew;
	}
	xRef=xNew;
}

double wrap180(double x)
{
	double y;
	y=mod180(modulus(x,360));
	return y;
	//double y=x;
	//while(fabs(y)>180.)
		//if(y>180.) y-=360.;
		//else y+=360.;
	//if(y==-180.) y=180.;
	//return y;
}

double modulus(double x,double y) {
	double m=floor(x/y);
	return (x-m*y);
}




