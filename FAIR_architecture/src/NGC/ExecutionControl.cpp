/*
 * ExecutionControl.cpp
 *
 *  Created on: 14 May 2024
 *      Author: massimo
 */

#include "ExecutionControl.h"

ExecutionControl::ExecutionControl(void) {
	mName="";
    //
    // Guidance & Control
    //
    mGuidance=NULL;
    mAutopilot=NULL;
    mControl=NULL;
}

ExecutionControl::~ExecutionControl(void) {
	if(mGuidance!=NULL) {
		mGuidance=NULL;
	}
	if(mControl!=NULL) {
		mControl=NULL;
	}
}

void ExecutionControl::reset(void) {
	mGuidance->reset();
	mControl->reset();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Guidance
//

bool ExecutionControl::enable_guidance(string  task) {
	//
	// check Guidance task
	//
	bool flag;
	flag=(task==PATH_FOLLOWING)||(task==LINE_FOLLOWING)||(task==GO_TO_HEADING)||(task==GO_TO_COURSE)||(task==KEEP_POINT);
	if(!flag) {
		printf("ExecutionControl::enable_guidance unexpected task %s\n",task.c_str());
		fflush(stdout);
		return false;
	}
	//
	// check selected guidance algo
	//
	string selectedGuidance=mGuidance->get_selected_algo(task);
	if(selectedGuidance!="null") {
		//
		// check dependencies
		//
		GuidanceControlAlgo* gAlgo=mGuidance->get(task, selectedGuidance);
		RequiredTaskList* requiredTaskList=NULL;
		if(gAlgo!=NULL)
			requiredTaskList=gAlgo->get_required_task_list();
		string requiredTask;
		bool dependenciesOk=true;
		bool autopilotIsRequired=false;
		for(int i=0;i<requiredTaskList->nTasks;i++) {
			requiredTask=requiredTaskList->mTask[i];
			bool flagAutopilot=enable_autopilot(requiredTask);
			bool flagControl=enable_control(requiredTask);
			dependenciesOk = dependenciesOk && (flagAutopilot||flagControl);
			autopilotIsRequired=autopilotIsRequired||flagAutopilot;
		}
		if(dependenciesOk) {
			if(!autopilotIsRequired)
				disable_autopilot();
			//
			// activate task
			//
			mGuidance->set_task(task);
			return true;
		}
		else return false;
	}
	else {
//		printf("ExecutionControl::enable_guidance task %s selected autopilot is %s\n",task.c_str(),selectedGuidance.c_str());
		printf("ExecutionControl::enable_guidance task %s selected guidance is %s\n",task.c_str(),selectedGuidance.c_str());
		fflush(stdout);
		return false;
	}
}



void ExecutionControl::disable_guidance(void) {
	GuidanceControlAlgo* activeGuidanceTask=mGuidance->get_active_guidance();
	if(activeGuidanceTask!=NULL) {
		//
		// disable task
		//
		mGuidance->set_task("null");
	}
}


bool ExecutionControl::set_guidance_algo(string task,string algo) {
	//
	// check guidance task
	//
	bool flag;
	flag=(task==PATH_FOLLOWING)||(task==LINE_FOLLOWING)||(task==GO_TO_HEADING)||(task==GO_TO_COURSE)||(task==KEEP_POINT);
	if(!flag) {
		printf("ExecutionControl::set_guidance_algo unexpected task %s\n",task.c_str());
		fflush(stdout);
		return false;
	}
	//
	// check if algo is "null"
	//	this implies to disable Guidance if the corresponding task is active
	//
	if(algo=="null") {
		GuidanceControlAlgo* activeGuidance=mGuidance->get_active_guidance();
		if(activeGuidance!=NULL) {
			if(activeGuidance->get_task()==task) {
				disable_guidance();
			}
		}
		mGuidance->set_selected_algo(task, algo);
		return true;
	}
	//
	// algo is NOT "null"
	//
	GuidanceControlAlgo* mAlgo;
	if((mAlgo=mGuidance->get(task, algo))!=NULL) {
		// set algo for the corresponding task
		mGuidance->set_selected_algo(task,algo);
		// reset algo
		mAlgo->reset();
		//
		// check if (task,algo) is already running
		//
		GuidanceControlAlgo* mActiveGuidance;
		mActiveGuidance=mGuidance->get_active_guidance();
		if(mAlgo==mActiveGuidance)
			return true;
		//
		// check if task is already running
		//
		if(mActiveGuidance!=NULL) {
			if(task==mActiveGuidance->get_task())
				// set guidance task with the new algorithm
				enable_guidance(task);
		}
		return true;
	}
	else {
		printf("ExecutionControl::set_guidance_algo unexpected task %s algorithm %s\n",task.c_str(),algo.c_str());
		fflush(stdout);
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Autopilot
//

bool ExecutionControl::enable_autopilot(string task) {
	//
	// check Autopilot task
	//
	bool flag;
	flag=(task==AUTO_HEADING)||(task==AUTO_COURSE);
	if(!flag) {
		printf("ExecutionControl::enable_autopilot unexpected task %s\n",task.c_str());
		fflush(stdout);
		return false;
	}
	//
	// check selected autopilot algo
	//
	string selectedAutopilot=mAutopilot->get_selected_algo(task);
	if(selectedAutopilot!="null") {
		//
		// check dependencies
		//
		GuidanceControlAlgo* apAlgo=mAutopilot->get(task, selectedAutopilot);
		RequiredTaskList* requiredTaskList=NULL;
		if(apAlgo!=NULL)
			requiredTaskList=apAlgo->get_required_task_list();
		string requiredTask;
		bool dependenciesOk=true;
		for(int i=0;i<requiredTaskList->nTasks;i++) {
			requiredTask=requiredTaskList->mTask[i];
			dependenciesOk = dependenciesOk && enable_control(requiredTask);
		}
		if(dependenciesOk) {
			//
			// disable Guidance
			//
			disable_guidance();
			//
			// activate task
			//
			mAutopilot->set_task(task);
			return true;
		}
		else return false;
	}
	else {
		printf("ExecutionControl::enable_autopilot task %s selected autopilot is %s\n",task.c_str(),selectedAutopilot.c_str());
		fflush(stdout);
		return false;
	}
}


void ExecutionControl::disable_autopilot(void) {
	GuidanceControlAlgo* activeAutopilotTask=mAutopilot->get_active_autopilot();
	if(activeAutopilotTask!=NULL) {
		//
		// disable dependencies
		//
		if(mGuidance->requires_task(activeAutopilotTask->get_task()))
			mGuidance->set_task("null");
		//
		// disable task
		//
		mAutopilot->set_task("null");
	}
}


bool ExecutionControl::set_autopilot_algo(string task,string algo) {
	//
	// check Autopilot task
	//
	bool flag;
	flag=(task==AUTO_HEADING)||(task==AUTO_COURSE);
	if(!flag) {
		printf("ExecutionControl::set_autopilot_algo unexpected task %s\n",task.c_str());
		fflush(stdout);
		return false;
	}
	//
	// check if algo is "null"
	//	this implies to disable Autopilot if the corresponding task is active
	//
	if(algo=="null") {
		GuidanceControlAlgo* activeAutopilot=mAutopilot->get_active_autopilot();
		if(activeAutopilot!=NULL) {
			if(activeAutopilot->get_task()==task) {
				disable_autopilot();
			}
		}
		mAutopilot->set_selected_algo(task, algo);
		return true;
	}
	//
	// algo is NOT "null"
	//
	GuidanceControlAlgo* mAlgo;
	if((mAlgo=mAutopilot->get(task, algo))!=NULL) {
		// set algo for the corresponding task
		mAutopilot->set_selected_algo(task,algo);
		// reset algo
		mAlgo->reset();
		//
		// check if (task,algo) is already running
		//
		GuidanceControlAlgo* mActiveAutopilot;
		mActiveAutopilot=mAutopilot->get_active_autopilot();
		if(mAlgo==mActiveAutopilot)
			return true;
		//
		// check if task is already running
		//
		if(mActiveAutopilot!=NULL) {
			if(task==mActiveAutopilot->get_task())
				// set autopilot task with the new algorithm
				enable_autopilot(task);
		}
		return true;
	}
	else {
		printf("ExecutionControl::set_autopilot_algo unexpected task %s algorithm %s\n",task.c_str(),algo.c_str());
		fflush(stdout);
		return false;
	}
//	return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Control
//

bool ExecutionControl::enable_control(string task) {
printf("ExecutionControl::enable_control begin \n");fflush(stdout);
	//
	// check Control task
	//
	bool flag;
	flag=(task==AUTO_SURGE)||(task==AUTO_SWAY)||(task==AUTO_YAW);
	if(!flag) {
		printf("ExecutionControl::set_control unexpected task %s\n",task.c_str());
		fflush(stdout);
		return false;
	}
	else {
		//
		// activate task
		//
		if(task==AUTO_SURGE) return mControl->enable_autosurge_task();
		if(task==AUTO_SWAY) return mControl->enable_autosway_task();
		if(task==AUTO_YAW) return  mControl->enable_autoyaw_task();
	}

printf("ExecutionControl::enable_control end \n");fflush(stdout);
	return false;
}

void ExecutionControl::disable_control(string task) {
	//
	// check Control task
	//
	bool flag;
	flag=(task==AUTO_SURGE)||(task==AUTO_SWAY)||(task==AUTO_YAW);
	if(!flag) {
		printf("ExecutionControl::disable_control unexpected task %s\n",task.c_str());
		fflush(stdout);
	}
	//
	// disable dependencies
	//
	if(mAutopilot->requires_task(task)) {
		// if requires task it is not NULL
		GuidanceControlAlgo* activeAutopilot=mAutopilot->get_active_autopilot();
		string autoPilotTask=activeAutopilot->get_task();
		if(mGuidance->requires_task(autoPilotTask))
			mGuidance->set_task("null");
		mAutopilot->set_task("null");
	}
	if(mGuidance->requires_task(task))
		mGuidance->set_task("null");
	//
	// disable task
	//
	if(task==AUTO_SURGE) mControl->disable_autosurge_task();
	if(task==AUTO_SWAY) mControl->disable_autosway_task();
	if(task==AUTO_YAW) mControl->disable_autoyaw_task();
}


bool ExecutionControl::set_control_algo(string task,string algo) {
	printf("******************** %s ---- ExecutionControl::set_control_algo %s %s\r\n",mName.c_str(),task.c_str(),algo.c_str());fflush(stdout);
	//
	// check Control task
	//
	bool flag;
	flag=(task==AUTO_SURGE)||(task==AUTO_SWAY)||(task==AUTO_YAW);
	if(!flag) {
		printf("ExecutionControl::set_control_algo unexpected task %s\n",task.c_str());
		fflush(stdout);
		return false;
	}
	//
	// check if algo is "null"
	//	this implies to disable the corresponding task
	//
	if(algo=="null") {
		disable_control(task);
		mControl->set_selected_algo(task,algo);
		return true;
	}
	//
	// algo is NOT "null"
	//
	GuidanceControlAlgo* mAlgo;
	if((mAlgo=mControl->get(task, algo))!=NULL) {
		// set algo for the corresponding task
		mControl->set_selected_algo(task,algo);
		// reset algo
		mAlgo->reset();
		//
		// check if (task,algo) is already running
		//
		GuidanceControlAlgo* mActiveController;
		if(task==AUTO_SURGE)
			mActiveController=mControl->get_active_autosurge();
		if(task==AUTO_SWAY)
			mActiveController=mControl->get_active_autosway();
		if(task==AUTO_YAW)
			mActiveController=mControl->get_active_autoyaw();

		if(mAlgo!=mActiveController) {
			//
			// check if task is already running
			//
			if(mActiveController!=NULL) {
				if(task==mActiveController->get_task())
					enable_control(task);
			}
		}
		return true;
	}
	else {
		printf("ExecutionControl::set_control_algo unexpected task %s algorithm %s\n",task.c_str(),algo.c_str());
		fflush(stdout);
		return false;
	}
}


void ExecutionControl::process_command(char *cmd) {
	printf("******************** %s ---- ExecutionControl::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="EXEC_CNTRL") {
		if(mExecCntrlCmd.decode(cmd, strCmd)) {
			if(strCmd=="ENABLE_GUIDANCE")
				process_enable_guidance();
			else if(strCmd=="DISABLE_GUIDANCE") {
				process_disable_guidance();
			}
			else if(strCmd=="ENABLE_AUTOPILOT")
				process_enable_autopilot();
			else if(strCmd=="DISABLE_AUTOPILOT") {
				process_disable_autopilot();
			}
			else if(strCmd=="ENABLE_CONTROL")
				process_enable_control();
			else if(strCmd=="DISABLE_CONTROL") {
				process_disable_control();
			}
			else if(strCmd=="SET_GUIDANCE_ALGO") {
				process_set_guidance_algo();
			}
			else if(strCmd=="SET_AUTOPILOT_ALGO") {
				process_set_autopilot_algo();
			}
			else if(strCmd=="SET_CONTROL_ALGO") {
				process_set_control_algo();
			}
			else  {
				printf("ExecutionControl::process_command ERROR read - ExecutionControl cmd is %s\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="EXEC_CNTRL")
	else  {
		printf("ExecutionControl::process_command ERROR read - type is %s\n",strType.c_str());
		fflush(stdout);
		return ;
	}
}


void ExecutionControl::process_enable_guidance(void) {
	enable_guidance(mExecCntrlCmd.task);
}


void ExecutionControl::process_disable_guidance(void) {
	disable_guidance();
}


void ExecutionControl::process_enable_autopilot(void) {
	enable_autopilot(mExecCntrlCmd.task);
}


void ExecutionControl::process_disable_autopilot(void) {
	disable_guidance();
	disable_autopilot();
}


void ExecutionControl::process_enable_control(void) {
	enable_control(mExecCntrlCmd.task);
}


void ExecutionControl::process_disable_control(void) {
	disable_guidance();
	disable_autopilot();
	disable_control(mExecCntrlCmd.task);
}


void ExecutionControl::process_set_guidance_algo(void) {
	set_guidance_algo(mExecCntrlCmd.task,mExecCntrlCmd.algo);
}


void ExecutionControl::process_set_autopilot_algo(void) {
	set_autopilot_algo(mExecCntrlCmd.task,mExecCntrlCmd.algo);
}


void ExecutionControl::process_set_control_algo(void) {
	printf("******************** %s ---- ExecutionControl::process_set_control_algo\r\n",mName.c_str());fflush(stdout);
	set_control_algo(mExecCntrlCmd.task,mExecCntrlCmd.algo);
}





