/*
 * Autopilot.h
 *
 *  Created on: 10 May 2024
 *      Author: massimo
 */

#ifndef AUTOPILOT_H_
#define AUTOPILOT_H_

#include <stdio.h>

#include <inttypes.h>
#include "NGCconfig.h"
#include "GuidanceControlReference.h"
#include "LogicalAttitudeData.h"
//#include "LogicalPositionData.h"
#include "LogicalSpeedData.h"
#include "GuidanceControlAlgo.h"
#include "PIautopilot.h"
#include "Commands.h"


class Autopilot {
private:
	string mName;
    //
    // Autopilot & Control Tasks references
    //
	// input
    AutopilotReference *mAutopilotRef;
    // output
    ControlReference *mCntrlRef;
    //
    // robot kinematics estimates
    //
//    LogicalPositionData *mPositionHat;
    LogicalAttitudeData *mAttitudeHat;
    LogicalSpeedData *mSpeedHat;

    //
    // Autopilot Algo
    //
    GuidanceControlAlgo* mAutopilot[N_MAX_ALGO];
    int nAutopilotAlgo;

    GuidanceControlAlgo* mActiveAutopilot;

	// selected task-algorithm tables
    //		column[0]	column[1]
    //		"task"		"algo"
    string selected_autopilot_algo[N_AUTOPILOT_TASK][2];

    AutopilotCmd mAutopilotCmd;

public:
	// constructor/destructor
    Autopilot();
    virtual ~Autopilot(void);
    virtual void reset(void);
    void set_name(string name) { mName=name; }
    // configure
    bool read_configuration(string filename);
    bool read_default_algorithms(string filename);
    // process commands
    void process_command(char *cmd);
    void process_set_heading(void);
    void process_set_course(void);
    // actions
    void set_task(string task);
    void set_io_ref(AutopilotReference *autopilotRef,ControlReference *cntrlRef);
    void set_estimates(LogicalAttitudeData *attitudeHat,LogicalSpeedData *speedHat);
    void set_selected_algo(string task,string algo);
    string get_selected_algo(string task);
    GuidanceControlAlgo* get(string task,string algo);
    GuidanceControlAlgo* get_active_autopilot(void) { return mActiveAutopilot; }
//    string get_active_autopilot(void) { if(mActiveAutopilot!=NULL) return mActiveAutopilot->get_task(); else return "null"; }
    bool requires_task(string task) {if(mActiveAutopilot!=NULL) return mActiveAutopilot->requires_task(task); else return false; }
    // execute
	void execute(void);

	void print(void);

private:
	bool instantiate_task(TaskAlgoInfo* taskAlgoInfo);protected:
	virtual void printf_results(void)  {printf("virtual void Autopilot::printf_results  %s\n",mName.c_str());fflush(stdout);}
};



#endif /* TCA_H_ */
