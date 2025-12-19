/*
 * GuidanceControlAlgo.h
 *
 *  Created on: 15 May 2024
 *      Author: massimo
 */

#ifndef GUIDANCECONTROLALGO_H_
#define GUIDANCECONTROLALGO_H_

#include <stdio.h>

#include <inttypes.h>
#include "GuidanceControlReference.h"
//#include "LinearPosition.h"
//#include "AngularPosition.h"
//#include "LinearVelocity.h"
//#include "AngularVelocity.h"

//
// Guidance tasks
//
#define N_GUIDANCE_TASK 5
#define PATH_FOLLOWING "path_following"
#define LINE_FOLLOWING "line_following"
#define GO_TO_HEADING "go_to_heading"
#define GO_TO_COURSE "go_to_course"
#define KEEP_POINT "keep_point"
//
// AutoPilot tasks
//
#define N_AUTOPILOT_TASK 2
#define AUTO_HEADING "auto_heading"
#define AUTO_COURSE "auto_course"
//
// Control tasks
//
#define N_CONTROL_TASK 3
#define AUTO_SURGE "auto_surge"
#define AUTO_SWAY "auto_sway"
#define AUTO_YAW "auto_yaw"
//
// TCA task
//
#define TCA "TCA"


#define N_MAX_ALGO 100

////
//// Algorithms
////
////#define NONE "null"
//// path following
//#define PF_VT_YAWRATE "PF_VT_yawrate" // virtual-target path-following setting yaw rate
//// line following
//#define LF_VT_YAWRATE "LF_VT_yawrate" // virtual-target line-following setting yaw rate
//#define LF_PD_YAWRATE "LF_PD_yawrate" // PD line-following setting yaw rate
//// go to
//#define LOS_HEADING "LOS_heading" // Line-Of-Sight go_to setting heading
//#define LOS_COURSE "LOS_course" // Line-Of-Sight go_to setting course
//// autopilot
//#define PI_SISO "PI_SISO" // 1 DOF PI controller

#define MAX_NEEDED_TASKS 10

typedef struct {
	string mTask[MAX_NEEDED_TASKS];
	int nTasks;
} RequiredTaskList;


typedef struct {
	string taskName;
	string algoName;
	string referenceOutput[MAX_NEEDED_TASKS];
	int nReferenceOutput;
	string estimateInput[MAX_NEEDED_TASKS];
	int nEstimateInput;
} TaskAlgoInfo;


class GuidanceControlAlgo {
protected:
	string mName;
	// algorithm task
	string mTask;
	// algorithm algo
	string mAlgo;
	// dependencies
	RequiredTaskList mRequiredTaskList;

    //
    // References
    // 		algorithm dependent
	//
	// input reference

	// output reference

    //
    // Estimates
    // 		algorithm dependent
    //


public:
	//
	// scheduling time
	//
	double mDt;

public:
	// constructor/destructor
	GuidanceControlAlgo();
    virtual ~GuidanceControlAlgo(void);

    virtual void set_input_output(double *d1,double *d2,double *d3) { printf("virtual void GuidanceControlAlgo::set_input_output\n"); fflush(stdout); }
    virtual void set_input_output(double *d1,double *d2,double *d3,double *d4) { printf("virtual void GuidanceControlAlgo::set_input_output\n"); fflush(stdout); }
    virtual void set_input_output(double *d1,double *d2,double *d3,double *d4,double *d5) { printf("virtual void GuidanceControlAlgo::set_input_output\n"); fflush(stdout); }
    virtual void set_input_output(double *d1,double *d2,double *d3,double *d4,double *d5,double *d6,double *d7) { printf("virtual void GuidanceControlAlgo::set_input_output\n"); fflush(stdout); }
    virtual void set_input_output(double *d1,double *d2,double *d3,double *d4,double *d5,
    		double *d6,double *d7,double *d8,double *d9,double *d10) { printf("virtual void GuidanceControlAlgo::set_input_output\n"); fflush(stdout); }

    void set_name(string name) { mName=name; }
    void set_task(string task) { mTask=task; }
    void set_algo(string algo) { mAlgo=algo; }
    string get_task(void) { return mTask; }
    string get_algo(void) { return mAlgo; }
    string get_name(void) { return mName; }
    bool requires_task(string task);
    RequiredTaskList* get_required_task_list(void) {return &mRequiredTaskList;}
    void add_required_task(string task) { mRequiredTaskList.mTask[mRequiredTaskList.nTasks++]=task; }

    void set_sampling_time(double dt) { mDt=dt; }

    virtual bool read_configuration(string fileName) {printf("virtual void GuidanceControlAlgo::read_configuration\n");fflush(stdout);return true;}
    virtual void reset(void) {printf("virtual void GuidanceControlAlgo::reset\n");fflush(stdout);}
	virtual void printf_parameters(void)  {printf("virtual void GuidanceControlAlgo::printf_parameters  %s\n",mName.c_str());fflush(stdout);}
    // execute
	virtual void execute(void)  {printf("virtual void GuidanceControlAlgo::execute\n");fflush(stdout);}
	//
    // void set_io_ref(...);
    // void set_estimates(..);
	// void set_parameters(...)
	// 		are algorithm dependent!
	//
	virtual void print(void);
};

void fread_string(FILE *f,string &str);
bool read_task_algo(FILE *f,TaskAlgoInfo *taskAlgoInfo,bool &flag);
bool read_task_algo(FILE *f,string &task,string &algo);
string get_required_task(string var);

#endif /* GUIDANCECONTROLALGO_H_ */
