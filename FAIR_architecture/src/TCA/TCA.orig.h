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
#include "ForceMoment.h"
#include "TCAstate.h"

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

class TCA {
protected:
	string mName;
	// Input
	ForceMoment *mForceMomentRef; // reference force and moment
	// Output
	ForceMoment *mForceMomentHat; // estimated force and moment
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
    virtual void reset(void) {mForceMomentRef->reset();mForceMomentHat->reset();mTCAstate->reset();}
    void set_name(string name) { mName=name; }

    // configure
    virtual void set_mode(string  str);
    virtual void set_function(string str) {mTCAstate->mFunction=str;printf("ThrustControlAllocation::set_function()\n");fflush(stdout);}
    virtual void set_config(string str) {mTCAstate->mConfig=str;printf("ThrustControlAllocation::set_config()\n");fflush(stdout);}
    void set_input_output(ForceMoment *forceMomentRef,ForceMoment *forceMomentHat);// {printf("a1\n");mForceTorqueRef=forceTorqueRef;printf("a2\n");mForceTorqueHat=forceTorqueHat;printf("a3\n");/*reset()*/;printf("a4\n");}
    void set_state(TCAstate *tcaState) { mTCAstate=tcaState; /*TCA::reset();*/ }

    virtual void set_manual_default(void) { printf("TCA::set_manual_default\n");fflush(stdout); }
    virtual void set_auto_default(void) { printf("TCA::set_auto_default\n");fflush(stdout); }

    //    void set_input_output(void) {printf("ThrustControlAllocation::set_input_output %s --- child class member function must be implmented!\n",mName.c_str());fflush(stdout);}
    // execute
	void execute(void); // {if(mMode==TCA_AUTO) execute_auto(); else if(mMode==TCA_MANUAL) execute_manual(); else execute_raw(); printf_results();}
protected:
	virtual void printf_results(void)  {printf("virtual void ThrustControlAllocation::printf_results  %s\n",mName.c_str());fflush(stdout);}
private:
	virtual void compute_applied_force_torque(void) {printf("virtual void ThrustControlAllocation::compute_applied_force_torque  %s\n",mName.c_str());fflush(stdout);}
    virtual void execute_auto(void) {printf("virtual void ThrustControlAllocation::execute_auto  %s\n",mName.c_str());fflush(stdout);}
    virtual void execute_manual(void) {printf("virtual void ThrustControlAllocation::execute_manual  %s\n",mName.c_str());fflush(stdout);}
    virtual void execute_raw(void) {printf("virtual void TCA::execute_raw  %s\n",mName.c_str());fflush(stdout);}
};



#endif /* TCA_H_ */
