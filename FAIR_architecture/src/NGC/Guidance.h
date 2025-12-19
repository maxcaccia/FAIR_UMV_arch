/*
 * Guidance.h
 *
 *  Created on: 10 May 2024
 *      Author: massimo
 */

#ifndef GUIDANCE_H_
#define GUIDANCE_H_

#include <stdio.h>

#include <inttypes.h>
#include "NGCconfig.h"
#include "GuidanceControlReference.h"
#include "LogicalAttitudeData.h"
//#include "LogicalAngularRateData.h"
#include "LogicalPositionData.h"
#include "LogicalLinearRateData.h"
//#include "LogicalSpeedData.h"
#include "GuidanceControlAlgo.h"
#include "LineOfSight.h"
#include "LineFollowingPD.h"
#include "Commands.h"


class Guidance {
private:
	string mName;
    //
    // Guidance, Autopilot & Control Tasks references
    //
	// input
    GuidanceReference *mGuidanceRef;
    // output
    AutopilotReference *mAutopilotRef;
    ControlReference *mCntrlRef;
    //
    // robot kinematics estimates
    //
    LogicalPositionData *mPositionHat;
    LogicalAttitudeData *mAttitudeHat;
    LogicalLinearRateData *mLinearRateHat;

    //
    // Guidance Task-Algorithms
    //
    GuidanceControlAlgo* mGuidance[N_MAX_ALGO];
    int nGuidanceAlgo;
    // active Guidance Task-Algorithm
    GuidanceControlAlgo* mActiveGuidance;

	// selected task-algorithm table
    //		column[0]	column[1]
    //		"task"		"algo"
    string selected_guidance_algo[N_GUIDANCE_TASK][2];

    GuidanceCmd mGuidanceCmd;

public:
	// constructor/destructor
	Guidance();
    virtual ~Guidance(void);
    virtual void reset(void);
    void set_name(string name) { mName=name; }
    // configure
    bool read_configuration(string filename);
    bool read_default_algorithms(string filename);
    // process commands
    void process_command(char *cmd);
    void process_set_keep_point(void);
    void process_set_waypoint(void);
    void process_set_line(void);
    void process_set_path(void);
    // actions
    void set_task(string  task);
    void set_io_ref(GuidanceReference *guidanceRef,AutopilotReference *autopilotRef,ControlReference *cntrlRef);
    void set_estimates(LogicalPositionData *positionHat,LogicalAttitudeData *attitudeHat,LogicalLinearRateData *linearRateHat);
    void set_selected_algo(string task,string algo);
    string get_selected_algo(string task);
    GuidanceControlAlgo* get(string task,string algo);
    GuidanceControlAlgo* get_active_guidance(void) { return mActiveGuidance; }
    bool requires_task(string task) {if(mActiveGuidance!=NULL) return mActiveGuidance->requires_task(task); else return false; }
    // execute
	void execute(void);

	void print(void);

private:
	bool instantiate_task(TaskAlgoInfo* taskAlgoInfo);
protected:
	virtual void printf_results(void)  {printf("virtual void Guidance::printf_results  %s\n",mName.c_str());fflush(stdout);}
};



#endif /* GUIDANCE_H_ */
