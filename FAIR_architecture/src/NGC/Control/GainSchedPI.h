/*
 * GainSchedPI.h
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#ifndef GAINSCHEDPI_H_
#define GAINSCHEDPI_H_

#include <stdio.h>
#include "GuidanceControlAlgo.h"
#include "BasicMath.h"


class GainSchedPI: public GuidanceControlAlgo
{
protected:
	// input: reference, estimate and exogenous input
	double *xRef,*xHat;
	// output
	double *yRef;

	// parameters: dynamics
	double mx;
	double kx;
	double kxx;
//	double kXx;

//	bool mDegreeFlag;

public:
	// task constructor
//	GainSchedPI(double dt,bool flag);
	GainSchedPI(double dt);
	~GainSchedPI(void) {};

//	void set_input_output(double *xRef,double *xHat,double *yRef,double *Xhat);
	void set_input_output(double *xRef,double *xHat,double *yRef);

	virtual void execute(void);

	virtual bool read_configuration(string filename);
	void set_parameters(double sigma,double omega,double deltaXmax,double ySat);
	void set_hydrodynamics_parameters(double mx,double kx,double kxx);
//	void set_degree_flag(bool flag) { mDegreeFlag=flag; }

	// procedure for resetting state - custom
	void reset(void);

	void print(void);

protected:
	// state: integrator value
	double Ie;
	// parameters
	double mSigma;
	double mOmega;
	double deltaXmax;
	double ySatMin;
	double ySatMax;

private:
	double compute_feedforward_action(double x) { double y=kx*x+kxx*x*fabs(x); return y; }
};


class GainSchedPIyaw: public GainSchedPI
{
private:
	// input: reference, estimate and exogenous input
	double *uHat;
	// parameters: dynamics
	double kux;

public:
	// task constructor
	GainSchedPIyaw(double dt);
	~GainSchedPIyaw(void) {};

	void set_input_output(double *xRef,double *xHat,double *yRef,double *uHat);
	void set_hydrodynamics_parameters(double mx,double kx,double kxx,double kux);

	void execute(void);

private:

private:
	double compute_feedforward_action(double x,double u) { double y=kx*x+kxx*x*fabs(x)+kux*u*x; return y; }
};


#endif /* GAINSCHEDPI_H_ */
