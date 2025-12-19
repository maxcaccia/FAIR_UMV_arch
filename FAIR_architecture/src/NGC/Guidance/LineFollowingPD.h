/*
 * LineFollowingPD.h
 *
 *  Created on: 16 July 2022
 *      Author: massimo
 */

#ifndef LINEFOLLOWINGPD_H_
#define LINEFOLLOWINGPD_H_

#include "GuidanceControlAlgo.h"
#include "BasicMath.h"
#include "MathAngles.h"

class LineFollowingPD: public GuidanceControlAlgo
{
private:
	// input: reference and estimate
	double *xL,*yL,*gammaL,*xHat,*yHat,*psiHat,*uBar,*uHat;
	// output
	double *yRef,*uRef;

public:
	// task constructor
	LineFollowingPD(void);
	~LineFollowingPD(void) {};

	void set_input_output(double *xL,double *yL,double *gammaL,double *xHat,double *yHat,double *psiHat,double *uBar,double *uHat,double *yRef,double *uRef);

	void execute(void);

	bool read_configuration(string fileName);
	void set_parameters(double sigma,double omega,double betaBar,double etaBar,double rSat,double uMax);

	void print(void);

private:
	// parameters
	double sigma;
	double omega;
	double betaBar;
	double etaBar;
	double rSat;
	double uMax;
};




#endif /* LINEFOLLOWINGPD_H_ */
