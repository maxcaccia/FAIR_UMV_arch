/*
 * TCAalgorithms.h
 *
 *  Created on: 22 July 2025
 *      Author: massimo
 */
#ifndef TCAALGORITHMS_H_
#define TCAALGORITHMS_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <nlopt.h>

#include "BasicMath.h"

typedef struct {
    double XtildeRef;
    double NtildeRef;
    double kDiff;
    double d;
    double l;
    double lambdaN;

} cost_surge_yaw_ref_params_t;

typedef struct {
	double XtildeRef;
    double YtildeRef;
    double NtildeRef;
    double d;
    double l;
    double lambdaY;
    double lambdaN;
    double lambdaT;
    double lambdaAlpha;
} cost_surge_sway_yaw_ref_params_t;


class TCAalgorithms {
public:
	// constructor/destructor
	TCAalgorithms();
    ~TCAalgorithms(void);

    double TCA_differential_steering(double Xtilde,double Ntilde,double &T1tilde,double &T2tilde);
    double TCA_thrust_azimuth_steering(double Xtilde,double Ntilde,double &Ttilde,double &alpha);
    double TCA_thrust_azimuth_diamond(double Xtilde,double Ytilde,double Ntilde,double *Ttilde,double &alpha);
    double TCA_thrust_azimuth_free(double Xtilde,double Ytilde,double Ntilde,double *Ttilde,double *alpha);

    void set_parameters(double k,double D,double L,double kY,double kN,double kT,double kAlpha);

private:
    void set_constraints(double kDiff,double d,double l);

    static double cost_surge_yaw_differential(unsigned n,const double *x,double *grad,void *data);
    static double cost_surge_yaw_thrust_azimuth(unsigned n,const double *x,double *grad,void *data);
    static double cost_surge_sway_yaw_thrust_azimuth_diamond(unsigned n,const double *x,double *grad,void *data);
    static double cost_surge_sway_yaw_thrust_azimuth_free(unsigned n,const double *x,double *grad,void *data);
    static double raw_cost_surge_sway_yaw_thrust_azimuth_diamond(unsigned n,const double *x,double *grad,void *data);
    static double raw_cost_surge_sway_yaw_thrust_azimuth_free(unsigned n,const double *x,double *grad,void *data);
private:
	//
	// optimisers
	//
	// surge-yaw differential propulsion
	nlopt_opt surge_yaw_differential_opt;
	// surge-yaw azimuth jets
	nlopt_opt surge_yaw_azimuth_opt;
	// surge-sway-yaw diamond
	nlopt_opt surge_sway_yaw_diamond_opt;
	// surge-sway-yaw free
	nlopt_opt surge_sway_yaw_free_opt;
    //
    // optimiser data/parameters
    //
	cost_surge_yaw_ref_params_t surge_yaw_ref_params;
	cost_surge_sway_yaw_ref_params_t surge_sway_yaw_ref_params;
	//
	// optimiser constraints
	//
	// surge-yaw differential propulsion
	double surge_yaw_differential_lb[2];
	double surge_yaw_differential_ub[2];
	// surge-yaw azimuth jets
	double surge_yaw_azimuth_lb[2];
	double surge_yaw_azimuth_ub[2];
	// surge-sway-yaw diamond
	double surge_sway_yaw_diamond_lb[5];
	double surge_sway_yaw_diamond_ub[5];
	// surge-sway-yaw free
	double surge_sway_yaw_free_lb[8];
	double surge_sway_yaw_free_ub[8];

	//
	// algo parameters
	//
	double kDiff;
	double d;
	double l;
    double lambdaY,lambdaN;
	double lambdaT,lambdaAlpha;
};

#endif /* TCAALGORITHMS_H_ */

