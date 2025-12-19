/*
 * LineOfSight.h
 *
 *  Created on: 11 giu 2022
 *      Author: massimo
 */

#ifndef LINEOFSIGHT_H_
#define LINEOFSIGHT_H_

#include <stdio.h>
#include "GuidanceControlAlgo.h"
#include "BasicMath.h"

class LineOfSight: public GuidanceControlAlgo
{
private:
	// input: reference and estimate
	double *latRef,*lonRef,*latHat,*lonHat,*uBar;
	// output
	double *psiRef,*uRef;

public:
	// task constructor
	LineOfSight(void);
	~LineOfSight(void) {};

	void set_input_output(double *latRef,double *lonRef,double *latHat,double *lonHat,double *uBar,double *psiRef,double *uRef);

	void execute(void);

	bool read_configuration(string filename);
	void set_parameters(double uMax,double ro) { this->uMax=uMax; roWP=ro;}

	void print(void);

private:
	// parameters
	double uMax;
	double roWP;
};


#endif /* LINEOFSIGHT_H_ */
