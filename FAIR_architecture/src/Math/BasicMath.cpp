/*
 * BasicMath.cpp
 *
 *  Created on: Dec 15, 2023
 *      Author: mc
 */

#include "BasicMath.h"
#include <cstdio>

double sat(double x,double min, double max)
{
	double y=x;
	if (x>max) y=max;
	if (x<min) y=min;
	return y;
}


double pythag(double a,double b)
// computes sqrt(a^2+b^2) without destructive underflow or overflow
{
	double absa,absb;

	absa=fabs(a);
	absb=fabs(b);
	if(absa>absb) return absa*sqrt(1.0+SQR(absb/absa));
	else return (absb==0 ? 0.0 : absb*sqrt(1.0+SQR(absa/absb)));
}

double DMAX(double a,double b)
{
	double dmaxarg1,dmaxarg2;
	return (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1) > (dmaxarg2) ? (dmaxarg1) : (dmaxarg2));
}

double DMIN(double a,double b)
{
	double dmaxarg1,dmaxarg2;
	return (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1) < (dmaxarg2) ? (dmaxarg1) : (dmaxarg2));
}

int IMIN(int a,int b)
{
	int iminarg1,iminarg2;
	return (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ? (iminarg1) : (iminarg2));
}


double SQR(double a)
{
	 double sqrarg;
	 return ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg);
}


double bilinear_interpolation(double z00,double z01, double z10, double z11, double dx, double dy) {
	return (1 - dx) * (1 - dy) * z00 +
	           dx * (1 - dy) * z10 +
	           (1 - dx) * dy * z01 +
	           dx * dy * z11;
}

double compute_propeller_thrust(double a,double k,double n) {
	double T;
	if(n>=0)
		T=a*n*n;
	else
		T=k*a*n*fabs(n);
	return T;
}

double compute_propeller_rpm(double a,double k,double T) {
	double n;
	if(T>=0)
		n=sqrt(T/a);
	else
		n=-sqrt(-T/(a*k));
		
//	printf("compute_propeller_rpm a %.2lf k %.2lf T %.3lf n %.3lf\n",a,k,T,n);fflush(stdout);

	return(n);
}
