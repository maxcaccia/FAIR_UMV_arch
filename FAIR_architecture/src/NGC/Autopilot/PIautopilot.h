/*
 * PIautopilot.h
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#ifndef PIAUTOPILOT_H_
#define PIAUTOPILOT_H_

#include "GuidanceControlAlgo.h"
#include "BasicMath.h"
#include "MathAngles.h"

class PIautopilot: public GuidanceControlAlgo
{
private:
	// input: reference and estimate
	double *xRef,*xHat,*uBar;
	// output
	double *yRef,*uRef;

public:
	// task constructor
	PIautopilot(double dt);
	~PIautopilot(void) {};

	void set_input_output(double *xRef,double *xHat,double *uBar,double *yRef,double *uRef);

	void execute(void);

	bool read_configuration(string fileName);
	void set_parameters(double kP,double gI,double Isat,double eIon,double eIoff,double ySat,double uMax);

	// procedure for resetting state - custom
	void reset(void);

	void print(void);

private:
	// state: integrator value
	double eI;
	// state: bool flag
	bool flagI;
	// parameters
	double kP;
	double gI;
	double Isat;
	double eIon; // eIoff>eIon
	double eIoff;
	double ySat;
	double uMax;
};




#endif /* PIAUTOPILOT_H_ */
