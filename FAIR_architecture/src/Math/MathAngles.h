/*
 * MathAngles.h
 *
 *  Created on: Oct 24, 2023
 *      Author: mc
 */

#ifndef MATHANGLES_H_
#define MATHANGLES_H_

#include <math.h>

double modpi(double a);
double mod180(double a);
double mod360(double x);
double summodpi(double a,double b);
double summod180(double a,double b);
double summod360(double a,double b);
double summod90(double a,double b);
void unwrap(double x,double &xRef);
double wrap180(double x);
double module(double x,double y);
double module360(double x);

#endif /* MATHANGLES_H_ */
