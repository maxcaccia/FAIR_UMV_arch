/*
 * SwampTCA.h
 *
 *  Created on: 5 mar 2024
 *      Author: massimo
 */

#ifndef SWAMPTCA_H_
#define SWAMPTCA_H_

#include <stdio.h>

#include <inttypes.h>
#include "TCA.h"
#include "SwampActuation.h"
#include "Matrix.h"

//
// Swamp TCA functions in TCA_AUTO mode and corresponding configurations:
//		"null"
//		"hovering" : "diamond" , "diamond_azimuth" , "free"
//		"steering" : "all" , "bow" , "stern" , "free"
//

//
// Swamp TCA functions in TCA_MANUAL mode and corresponding configurations:
//		"null"
//		"translation" : "parallel"
//		"forward_steering" : "all" , "bow" , "stern"
//		"backward_steering" : "all" , "bow" , "stern"
//

//
// actuators & allocation parameters
//
#define SWAMP_N_MAX 2000.
//#define SWAMP_A_THRUST 0.000004087 // thrust[i] = aThrust * n[i]^2
#define SWAMP_A_THRUST 0.0000033010525 // thrust[i] = aThrust * n[i]^2
#define SWAMP_THRUST_MAX 10.00
#define SWAMP_L 0.35
#define SWAMP_D 0.45

//
// power consumption weight
//
#define SWAMP_WEIGHT_POWER 0.001
//
// X, Y, N weight (priority)
//
#define SWAMP_X_WEIGHT 1.
#define SWAMP_Y_WEIGHT 1.
#define SWAMP_N_WEIGHT 1.


class SwampTCA : public TCA {
private:
	// Input
	SwampActuation* mSwampActuationMeas; // measured robot actuation

	// Output
	SwampActuation* mSwampActuationRef; // reference robot actuation

public:
	// constructor/destructor
	SwampTCA(void);
    virtual ~SwampTCA(void);
    void reset(void);
    // configure
    void set_mode(string  str);
    void set_function(string str);
    void set_configuration(string str);
    void set_rpm_azimuth(double n,double alpha) { mSwampActuationRef->n=n; mSwampActuationRef->alpha=alpha; }

//    void set_input_output(ForceTorque *forceTorque,SwampActuation* swampActuation) {mForceTorqueRef=forceTorque;mSwampActuationRef=swampActuation;}
    void set_input_output(SwampActuation* swampActuationRef,SwampActuation* swampActuationMeas); // {mSwampActuationRef=swampActuationRef;mSwampActuationMeas=swampActuationMeas;SwampTCA::reset();}

    void set_manual_default(void);
    void set_auto_default(void);

private:
    void compute_applied_force_torque(void);
    void printf_results(void);
    void execute_manual(void);
    void execute_auto(void);

    //
    // manual TCA
    //
    void translation_mapping(void);
    void forward_XN_all_mapping(void);
    void forward_XN_bow_mapping(void);
    void forward_XN_stern_mapping(void);
    void backward_XN_all_mapping(void);
    void backward_XN_bow_mapping(void);
    void backward_XN_stern_mapping(void);

    //
    // auto TCA
    //
    // [X,Y,N] --> all
    void auto_hovering(void);
    void auto_hovering_azimuth_fixed(void);
    // [X,N] --> all
    void auto_XN_all(void);
    void auto_forward_XN_all(void);
    void auto_backward_XN_all(void);
    void auto_XN_all_n_alpha(void);
    // [X,N] --> bow
    void auto_XN_bow(void);
    void auto_forward_XN_bow(void);
    void auto_backward_XN_bow(void);
    void auto_XN_2_n_alpha(void);
    // [X,N] --> stern
    void auto_XN_stern(void);
    void auto_forward_XN_stern(void);
    void auto_backward_XN_stern(void);

    double optimise_hovering_azimuth_fixed(double *T,double alphaDeg);

    void allocate_azimuth_diamond(void);

private:
//    //
//    // actuators & allocation parameters
//    //
//    double a;
//    double Tmax;
//    double l;
//    double d;

};



#endif /* SWAMPTCA_H_ */
