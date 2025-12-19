/*
 * Control.h
 *
 *  Created on: 10 May 2024
 *      Author: massimo
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdio.h>

#include <inttypes.h>
#include "NGCconfig.h"
#include "GuidanceControlReference.h"
#include "ForceMoment.h"
#include "LogicalLinearRateData.h"
#include "LogicalAngularRateData.h"
#include "HydroDynamics.h"
#include "GuidanceControlAlgo.h"
#include "GainSchedPI.h"
#include "Commands.h"


class Control {
private:
	string mName;
    //
    // Control and Force/Torque references
    //
    ControlReference *mCntrlRef;
    ForceMoment *mForceMomentRef;
    //
    // robot kinematics estimates
    //
    LogicalLinearRateData *mLinearRateHat; // linear velocity
    LogicalAngularRateData *mAngularRateHat; // angular velocity
    //
    // robot hydrodynamics
    //
    HydroDynamics* mHydroPar;

    //
    // Control Task-Algorithms
    //
    GuidanceControlAlgo* mControl[N_MAX_ALGO];
    int nControlAlgo;
    // active Control Task-Algorithm
    GuidanceControlAlgo* mActiveAutoSurge;
    GuidanceControlAlgo* mActiveAutoSway;
    GuidanceControlAlgo* mActiveAutoYaw;

	// selected task-algorithm table
    //		column[0]	column[1]
    //		"task"		"algo"
    string selected_control_algo[N_CONTROL_TASK][2];

    ControlCmd mCntrlCmd;

public:
	// constructor/destructor
	Control();
    virtual ~Control(void);
    virtual void reset(void);
    void set_name(string name) { mName=name; }
    // configure
    bool read_configuration(string filename);
    bool read_default_algorithms(string filename);
    // process commands
    void process_command(char *cmd);
    void process_set_surge(void);
    void process_set_sway(void);
    void process_set_yaw(void);
    // actions
    bool enable_autosurge_task(void);
    bool enable_autosway_task(void);
    bool enable_autoyaw_task(void);
    void disable_autosurge_task(void) { if(mActiveAutoSurge!=NULL) mActiveAutoSurge->reset(); mActiveAutoSurge=NULL; }
    void disable_autosway_task(void) { if(mActiveAutoSway!=NULL)  mActiveAutoSway->reset(); mActiveAutoSway=NULL; }
    void disable_autoyaw_task(void) { if(mActiveAutoYaw!=NULL)  mActiveAutoYaw->reset(); mActiveAutoYaw=NULL; }
    void set_io_ref(ControlReference *cntrlRef,ForceMoment *forceMomentRef);
    void set_estimates(LogicalLinearRateData *linearRateHat,LogicalAngularRateData *angularRateHat);
    void set_hydrodynamics(HydroDynamics *hydroPar) { mHydroPar=hydroPar; }
    void set_selected_algo(string task,string algo);
    string get_selected_algo(string task);
    GuidanceControlAlgo* get(string task,string algo);
    GuidanceControlAlgo* get_active_autosurge(void) { return mActiveAutoSurge; }
    GuidanceControlAlgo* get_active_autosway(void) { return mActiveAutoSway; }
    GuidanceControlAlgo* get_active_autoyaw(void) { return mActiveAutoYaw; }
//    bool is_active(string task);
    // execute
	void execute(void);

	void print(void);
	void print_selected_algo(void);

private:
	bool instantiate_task(TaskAlgoInfo* taskAlgoInfo);protected:
	virtual void printf_results(void)  {printf("virtual void Control::printf_results  %s\n",mName.c_str());fflush(stdout);}
};



#endif /* CONTROL_H_ */
