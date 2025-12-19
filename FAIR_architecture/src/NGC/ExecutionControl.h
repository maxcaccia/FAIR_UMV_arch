/*
 * ExecutionControl.h
 *
 *  Created on: 14 May 2024
 *      Author: massimo
 */

#ifndef EXECUTIONCONTROL_H_
#define EXECUTIONCONTROL_H_

#include <stdio.h>

#include <inttypes.h>
#include "Guidance.h"
#include "Autopilot.h"
#include "Control.h"


class ExecutionControl {
private:
	string mName;
    //
    // Guidance, Autopilot & Control
    //
    Guidance *mGuidance;
    Autopilot *mAutopilot;
    Control *mControl;

    ExecCntrlCmd mExecCntrlCmd;

public:
	// constructor/destructor
    ExecutionControl();
    virtual ~ExecutionControl(void);
    void reset(void);
    void set_name(string name) { mName=name; }
    void init(Guidance *guidance,Autopilot *autopilot,Control *control) { mGuidance=guidance;mAutopilot=autopilot;mControl=control; }
    // process commands
    void process_command(char *cmd);
    void process_enable_guidance(void);
    void process_disable_guidance(void);
    void process_enable_autopilot(void);
    void process_disable_autopilot(void);
    void process_enable_control(void);
    void process_disable_control(void);
    void process_set_guidance_algo(void);
    void process_set_autopilot_algo(void);
    void process_set_control_algo(void);
    // actions
    bool enable_guidance(string task);
    bool enable_autopilot(string task);
    bool enable_control(string task);
    void disable_guidance(void);
    void disable_autopilot(void);
    void disable_control(string task);

    bool set_guidance_algo(string task,string algo);
    bool set_autopilot_algo(string task,string algo);
    bool set_control_algo(string task,string algo);
};



#endif /* EXECUTIONCONTROL_H_ */
