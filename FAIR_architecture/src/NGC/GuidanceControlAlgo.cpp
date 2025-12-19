/*
 * GuidanceControlAlgo.cpp
 *
 *  Created on: 17 May 2024
 *      Author: massimo
 */

#include "GuidanceControlAlgo.h"

GuidanceControlAlgo::GuidanceControlAlgo(void) {
	mName="";
	// algorithm task
	mTask="";
	// algorithm algo
	mAlgo="";
	// dependencies
	RequiredTaskList mRequiredTaskList;
    for(int i=0;i<MAX_NEEDED_TASKS;i++) {
    	mRequiredTaskList.mTask[i]="";
    }
    mRequiredTaskList.nTasks=0;

    mDt=0.;
}


GuidanceControlAlgo::~GuidanceControlAlgo(void) {

}


bool GuidanceControlAlgo::requires_task(string task) {
	bool flag=false;
	for(int i=0;i<mRequiredTaskList.nTasks;i++) {
		if(mRequiredTaskList.mTask[i]==task)
			flag=true;
	}
	return flag;
}


void GuidanceControlAlgo::print(void) {
	printf("GuidanceControlAlgo::print %s task %s algo %s\n",mName.c_str(),mTask.c_str(),mAlgo.c_str());
	printf("\trequires tasks:\n");
	for(int i=0;i<mRequiredTaskList.nTasks;i++)
		printf("\t\t%s\n",mRequiredTaskList.mTask[i].c_str());
	fflush(stdout);
}


void fread_string(FILE *f,string &str) {
	char s[256];fscanf(f,"%s",s);str=s;
}


bool read_task_algo(FILE *f,TaskAlgoInfo *taskAlgoInfo,bool &flag) {
	string str;
	string name;

	flag=false;
	//
	// read task name
	//
	fread_string(f,str);
//	printf("read_task_algo %s\n",str.c_str());
	if(str=="#end") {
		// End-OF-File
		flag=true;
		return false;
	}
	if(str!="#task") {
		printf("read_task_algo ERROR - #task expected %s found\n",str.c_str());
		return false;
	}
	fread_string(f,taskAlgoInfo->taskName);
//	printf("read_task_algo %s\n",taskAlgoInfo->taskName.c_str());
	//
	// read algo name
	//
	fread_string(f,str);
//	printf("read_task_algo %s\n",str.c_str());
	if(str!="#algo") {
		printf("read_task_algo ERROR - #algo expected %s found\n",str.c_str());
		return false;
	}
	fread_string(f,taskAlgoInfo->algoName);
//	printf("read_task_algo %s\n",taskAlgoInfo->algoName.c_str());
	//
	// read reference output
	//
	fread_string(f,str);
//	printf("read_task_algo %s\n",str.c_str());
	if(str!="#reference_output") {
		printf("read_task_algo ERROR - #reference_output expected %s found\n",str.c_str());
		return false;
	}
	fscanf(f,"%d",&(taskAlgoInfo->nReferenceOutput));
//	printf("read_task_algo %d\n",taskAlgoInfo->nReferenceOutput);
	for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
		fread_string(f,taskAlgoInfo->referenceOutput[i]);
//		printf("read_task_algo %s\n",taskAlgoInfo->referenceOutput[i].c_str());
	}
	//
	// read estimate input
	//
	fread_string(f,str);
//	printf("read_task_algo %s\n",str.c_str());
	if(str!="#estimate_input") {
		printf("read_task_algo ERROR - #estimate_input expected %s found\n",str.c_str());
		return false;
	}
	fscanf(f,"%d",&(taskAlgoInfo->nEstimateInput));
//	printf("read_task_algo %d\n",taskAlgoInfo->nEstimateInput);
	for(int i=0;i<taskAlgoInfo->nEstimateInput;i++) {
		fread_string(f,taskAlgoInfo->estimateInput[i]);
//		printf("read_task_algo %s\n",taskAlgoInfo->estimateInput[i].c_str());
	}
	//
	// read end task
	//
	fread_string(f,str);
//	printf("read_task_algo %s\n",str.c_str());
	if(str!="#endtask") {
		printf("read_task_algo ERROR - #endtask expected %s found\n",str.c_str());
		return false;
	}

	return true;
}

bool read_task_algo(FILE *f,string &task,string &algo) {
	string str;

	fread_string(f,str);
	if(str=="#end") // end-of-file
		return false;
	if(str=="#task-algo") {
		// read default task-algo
		fread_string(f,task);
		fread_string(f,algo);
		return true;
	}
	printf("ERROR - read_task_algo(FILE *f,string &task,string &algo) wrong format %s\n",str.c_str());
	return false;
}


string get_required_task(string var) {
	// Control
	if(var=="surge")
		return AUTO_SURGE;
	if(var=="sway")
		return AUTO_SWAY;
	if(var=="yawrate")
		return AUTO_YAW;
	// Autopilot
	if(var=="heading")
		return AUTO_HEADING;
	if(var=="course")
		return AUTO_COURSE;
	if(var=="surge_force")
		return TCA;
	if(var=="sway_force")
		return TCA;
	if(var=="yaw_torque")
		return TCA;
	return "";
}


