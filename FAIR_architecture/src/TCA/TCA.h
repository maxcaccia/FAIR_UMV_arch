/*
 * TCA.h
 *
 *  Created on: 5 mar 2024
 *      Author: massimo
 */

#ifndef TCA_H_
#define TCA_H_

#include <stdio.h>

#include <inttypes.h>
#include "XYN.h"
#include "XYNnorm.h"
#include "TCAstate.h"
#include "TCAalgorithms.h"
#include "MathAngles.h"
#include "Commands.h"

//
// Thrust Control Allocation mode
//
#define TCA_AUTO "auto"
#define TCA_MANUAL "manual"
#define TCA_RAW "raw"
//
// TCA auto functions are vehicle dependent and defined in <Robot>ThrustControlAllocation.h
//
//
// TCA auto configurations are vehicle dependent and defined in <Robot>ThrustControlAllocation.h
//

#define N_ACTUATORS 4

#define NOP_THRESHOLD 200

class TCA {
protected:
	int mNopCounter;
	bool mNopHandlerEnable;
	string mName;
	// Input
	XYN *mXYNref; // reference force and moment
	XYNnorm *mXYNnormRef; // reference normalised force and moment
	// Output
	XYN *mXYNhat; // estimated force and moment
    XYNnorm *mXYNnormHat; // estimated normalised force and moment
	// Input
	// <Robot>Ref m<Robot>Ref; // reference for <robot> MANUAL mode
	// Output
	// <Robot>Actuation m<Robot>ActRef; // reference robot actuation

	// state
	TCAstate *mTCAstate;
//	// TCA mode: "auto" / "manual" / "raw"
//	string mMode;
//	// TCA function;
//	string mFunction; // values are vehicle-dependent, e.g. "hovering","forward_steering",...
//	// TCA configuration
//	string mConfig; // values are vehicle-dependent, e.g. "diamond","all","bow","stern","free",...

public:
	// constructor/destructor
	TCA();
    virtual ~TCA(void);
    virtual void reset(void) {mXYNref->reset();mXYNnormRef->reset();mXYNhat->reset();mXYNnormHat->reset();/*mTCAstate->reset();*/}
    void clear_nop(void) { mNopCounter=0; mNopHandlerEnable=true;}
    void set_name(string name) { mName=name; }
    void set_config_dir(string configDir) { mConfigDir=configDir; }
    bool read_configuration(void);
//    bool fread(string filename);

    virtual void process_command(char *cmd,ssize_t n);
    virtual bool process_data(char *data,ssize_t nRecv) { printf("virtual void TCA::process_data %s\n",mName.c_str());fflush(stdout);return false; }

    // set reference
    void set_3_dof(double X,double Y,double N);
    void set_2_dof(double X,double N);
    void set_3_dof_norm(double Xnorm,double Ynorm,double Nnorm);
    void set_2_dof_norm(double Xnorm,double Nnorm);

    // configure
    void set_propulsion_class(string str);
    void set_config(string str);
    void set_mode(string  str);
    void set_function(string str);
    void set_input_output(XYN *XYNref,XYNnorm *XYNnormRef,XYN *XYNhat,XYNnorm *XYNnormHat);// {printf("a1\n");mForceTorqueRef=forceTorqueRef;printf("a2\n");mForceTorqueHat=forceTorqueHat;printf("a3\n");/*reset()*/;printf("a4\n");}
    void set_state(TCAstate *tcaState) { mTCAstate=tcaState; /*TCA::reset();*/ }

    virtual void set_manual_default(void) { printf("TCA::set_manual_default\n");fflush(stdout); }
    virtual void set_auto_default(void) { printf("TCA::set_auto_default\n");fflush(stdout); }
    virtual void start(void) { printf("TCA::start\n");fflush(stdout); }
    virtual void stop(void) { printf("TCA::stop\n");fflush(stdout); }

    //    void set_input_output(void) {printf("ThrustControlAllocation::set_input_output %s --- child class member function must be implmented!\n",mName.c_str());fflush(stdout);}
    // execute
    virtual void execute(void); // {if(mMode==TCA_AUTO) execute_auto(); else if(mMode==TCA_MANUAL) execute_manual(); else execute_raw(); printf_results();}
protected:
	virtual void printf_results(void)  {printf("virtual void ThrustControlAllocation::printf_results  %s\n",mName.c_str());fflush(stdout);}
private:
	virtual void compute_applied_force_torque(void) {printf("virtual void ThrustControlAllocation::compute_applied_force_torque  %s\n",mName.c_str());fflush(stdout);}
protected:
    virtual void execute_auto(void); // {printf("virtual void ThrustControlAllocation::execute_auto  %s\n",mName.c_str());fflush(stdout);}
    virtual void execute_manual(void) {printf("virtual void ThrustControlAllocation::execute_manual  %s\n",mName.c_str());fflush(stdout);}
    virtual void execute_raw(void) {printf("virtual void TCA::execute_raw  %s\n",mName.c_str());fflush(stdout);}
    virtual void handle_nop_emergency(void) {printf("virtual void TCA::handle_nop_emergency  %s\n",mName.c_str());fflush(stdout);}

protected:
    //
    // TCA algorithms
    //
    TCAalgorithms mTCAalgo;
    //
    // parameters
    //
    double Tmax;
    double kDiff;
    double d,l;
    double lambdaY,lambdaN;
	double lambdaT,lambdaAlpha;
    //
    // general thrust allocation
    //
    double mThrust[N_ACTUATORS]; // [N]
    double mThrustNorm[N_ACTUATORS]; // [-1,1]
    double mAzimuth[N_ACTUATORS]; // [deg]

    void standard_to_normalised_ref(void);
    void normalised_to_standard_ref(void);
    void normalised_to_standard_hat(void);

protected:
    string mConfigDir;
    //
    // TCA command
    //
    TcaCmd mTcaCmd;
};



#endif /* TCA_H_ */
