/*
 * BasicMath.h
 *
 *  Created on: Dec 15, 2023
 *      Author: mc
 */

#ifndef BASICMATH_H_
#define BASICMATH_H_

#include <math.h>

double sat(double x,double min, double max);

double pythag(double a,double b);

double DMAX(double a,double b);
double DMIN(double a,double b);
double max_vect(double *v,int n);
double min_vect(double *v,int n);
int IMIN(int a,int b);
double SQR(double a);

double bilinear_interpolation(double z00,double z01, double z10, double z11, double dx, double dy);
double compute_propeller_thrust(double a,double k,double n);
double compute_propeller_rpm(double a,double k,double T);

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define sign(a) ((a) >= 0.0 ? 1.0 : -1.0)

#endif /* BASICMATH_H_ */
