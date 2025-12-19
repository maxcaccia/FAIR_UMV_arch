/*
 * Autopilot.cpp
 *
 *  Created on: 14 May 2024
 *      Author: massimo
 */

#include "Autopilot.h"

Autopilot::Autopilot(void) {
	mName="";
    //
    // Autopilot & Control Tasks references
    //
    mAutopilotRef=NULL;
    mCntrlRef=NULL;
    //
    // robot kinematics estimates
    //
    mAttitudeHat=NULL;
    mSpeedHat=NULL;

    //
    // Autopilot Algo
    //
    for(int i=0;i<N_MAX_ALGO;i++) {
    		mAutopilot[i]=NULL;
    }
    nAutopilotAlgo=0;

    mActiveAutopilot=NULL;

    //
    // build selected task-algorithm table
    //
    selected_autopilot_algo[0][0]=AUTO_HEADING;
    selected_autopilot_algo[1][0]=AUTO_COURSE;
    for(int i=0;i<N_AUTOPILOT_TASK;i++)
    	selected_autopilot_algo[i][1]="null";

}

void Autopilot::reset(void) {
	// clean select task-algorithm table
	for(int i=0;i<N_AUTOPILOT_TASK;i++)
		selected_autopilot_algo[i][1]="null";

	// clean active Guidance
	mActiveAutopilot=NULL;

	// reset all Guidance algorithms
    for(int i=0;i<nAutopilotAlgo;i++) {
    		mAutopilot[i]->reset();
    }
}

Autopilot::~Autopilot(void) {
	if(mAutopilotRef!=NULL) {
		mAutopilotRef=NULL;
	}
	if(mCntrlRef!=NULL) {
		mCntrlRef=NULL;
	}
	if(mAttitudeHat!=NULL) {
		mAttitudeHat=NULL;
	}
}


bool Autopilot::read_configuration(string filename) {
	FILE *f;
	TaskAlgoInfo taskAlgoInfo;
	bool flag=false;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("Autopilot::read_configuration : ERROR opening file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}

	//
	// read header
	//
	string str;
	// read ======
	fread_string(f,str);
	// read autopilot tag
	fread_string(f,str);
//	printf("Autopilot::read_configuration %s\n",str.c_str());
	if(str!="#autopilot") {
		printf("Autopilot::read_configuration ERROR - #autopilot expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	// read autopilot name
	fread_string(f,mName);
//	printf("Autopilot::read_configuration %s\n",mName.c_str());
	// read ======
	fread_string(f,str);
//	printf("Autopilot::read_configuration %s\n",str.c_str());

	while(read_task_algo(f,&taskAlgoInfo,flag)) {
		// instantiate Autopilot task algo
//		printf("Autopilot::read_configuration - instantiate task %s algo %s\n",taskAlgoInfo.taskName.c_str(),taskAlgoInfo.algoName.c_str());
//		fflush(stdout);
		if(!instantiate_task(&taskAlgoInfo)) {
			fclose(f);
			return false;
		}
	}
	if(!flag) {
		printf("Autopilot::read_configuration( %s ) failed!\n",filename.c_str());
		fflush(stdout);
		fclose(f);
		return false;
	}

	fclose(f);
	return true;
}


bool Autopilot::read_default_algorithms(string filename) {
	FILE *f;
	TaskAlgoInfo taskAlgoInfo;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("Autopilot::read_default_algorithms : ERROR opening file %s\n",filename.c_str());
		fflush(stdout);
		fclose(f);
		return false;
	}

	//
	// read header
	//
	string str;
	// read ======
	fread_string(f,str);
	// read control tag
	fread_string(f,str);
	if(str!="#autopilot") {
		printf("Autopilot::read_default_algorithms ERROR - #control expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	// read control name
	fread_string(f,mName);
	// read ======
	fread_string(f,str);

	string task,algo;
	while(read_task_algo(f,task,algo)) {
		// check if (task,algo) has been instantiated
		bool flag=false;
		for(int i=0;i<nAutopilotAlgo;i++)
			flag=flag||((mAutopilot[i]->get_task()==task)&&(mAutopilot[i]->get_algo()==algo));
		if(flag) {
			// set default task algo
			printf("Autopilot::read_default_algorithms %s %s\n",task.c_str(),algo.c_str());
			set_selected_algo(task,algo);
		}
		else {
			printf("Autopilot::read_default_algorithms ERROR - (%s,%s) not instantiated\n",task.c_str(),algo.c_str());
			fclose(f);
			return false;
		}

	}

	fclose(f);
	return true;
}

GuidanceControlAlgo* Autopilot::get(string task,string algo) {
	for(int i=0;i<nAutopilotAlgo;i++) {
		if((mAutopilot[i]->get_task()==task)&&(mAutopilot[i]->get_algo()==algo))
			return mAutopilot[i];
	}

	return NULL;
}

void Autopilot::set_selected_algo(string task,string algo) {
	for(int i=0;i<N_AUTOPILOT_TASK;i++)
		if(selected_autopilot_algo[i][0]==task)
			selected_autopilot_algo[i][1]=algo;
}

string Autopilot::get_selected_algo(string task) {
	for(int i=0;i<N_AUTOPILOT_TASK;i++)
		if(selected_autopilot_algo[i][0]==task)
			return selected_autopilot_algo[i][1];
	printf("Autopilot::get_selected_algo unexpected task %s\n",task.c_str());
	fflush(stdout);
	return("null");
}


void Autopilot::set_task(string  task) {
	if(task=="null") {
		if(mActiveAutopilot!=NULL)
			mActiveAutopilot->reset();
		mActiveAutopilot=NULL;
		return ;
	}
	// Guidance Task: "PF" / "LF" / "go_to" / "keep_point" / "null"
	if((task==AUTO_HEADING)||(task==AUTO_COURSE)) {
		string algo=get_selected_algo(task);
		if(algo!="null") {
			GuidanceControlAlgo* apAlgo=get(task,algo);
			if(apAlgo!=NULL) {
				mActiveAutopilot=apAlgo;
				mActiveAutopilot->reset();
			}
			else {
				printf("Autopilot::set_guidance_task unexpected task %s algo %s\n",task.c_str(),algo.c_str());
				fflush(stdout);
			}
		}
		else {
			printf("Autopilot::set_guidance_task task %s selected algo is %s\n",task.c_str(),algo.c_str());
			fflush(stdout);
		}
	}
	else {
		printf("Autopilot::set_guidance_task unexpected task %s\n",task.c_str());
		fflush(stdout);
	}
}



void Autopilot::set_io_ref(AutopilotReference *autopilotRef,ControlReference *cntrlRef) {
	mAutopilotRef=autopilotRef;
	mCntrlRef=cntrlRef;
}

void Autopilot::set_estimates(LogicalAttitudeData *attitudeHat,LogicalSpeedData *speedHat) {
	mAttitudeHat=attitudeHat;
	mSpeedHat=speedHat;
}


void Autopilot::execute(void) {
	if(mActiveAutopilot!=NULL)
		mActiveAutopilot->execute();
}



void Autopilot::print(void) {
	printf("===================================\n");
	printf("===   Autopilot\n");
	printf("===================================\n");
	//
	// print Autopilot task-algo
	//
	for(int i=0;i<nAutopilotAlgo;i++)
		mAutopilot[i]->print();
	fflush(stdout);
}

bool Autopilot::instantiate_task(TaskAlgoInfo* taskAlgoInfo) {
//printf("Autopilot::instantiate_task %s %s\n",taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
	//
	// instantiate algo: PI_autopilot
	//
	GuidanceControlAlgo* apAlgo=NULL;
	if(taskAlgoInfo->algoName == "PI_autopilot") {
		// instantiate algo - PI_autopilot
		apAlgo = new PIautopilot(NGC_DELTA_T);
	}
	else {
		printf("Autopilot::instantiate_task - ERROR : unexpected algo %s\n",taskAlgoInfo->algoName.c_str());
		fflush(stdout);
		return false;
	}
	//
	// set name, task, algo
	//
	apAlgo->set_algo(taskAlgoInfo->algoName);
	apAlgo->set_task(taskAlgoInfo->taskName);
	apAlgo->set_name(taskAlgoInfo->taskName+"_"+taskAlgoInfo->algoName);
	// read algo configuration, i.e. parameters
	string s="GuidanceControl/";
	string filename=NGC_CONFIG_PATH+s+apAlgo->get_name()+".cfg";
	printf("Autopilot::instantiate_task read_configuration %s\n",filename.c_str());fflush(stdout);
	if(!apAlgo->read_configuration(filename)) {
		printf("Autopilot::instantiate_task error reading configuration file %s\n",filename.c_str());fflush(stdout);
		return false;
	}
	// set dependencies (reference)
	for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
		string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
		apAlgo->add_required_task(task);
	}
	// set dependencies (estimate): to be done
	//
	// set task : AUTO_HEADING , AUTO_COURSE
	//			I/O variables
	//
	if(taskAlgoInfo->taskName == AUTO_HEADING) {
		// set I/O variables
		apAlgo->set_input_output(&(mAutopilotRef->psi), &(mAttitudeHat->psi), &(mAutopilotRef->uBar),
				&(mCntrlRef->r), &(mCntrlRef->u));
	}
	else if(taskAlgoInfo->taskName == AUTO_COURSE) {
		// set I/O variables
		apAlgo->set_input_output(&(mAutopilotRef->chi), &(mSpeedHat->track), &(mAutopilotRef->uBar),
				&(mCntrlRef->r), &(mCntrlRef->u));
	}
	else {
		printf("Autopilot::instantiate_task - ERROR : unexpected task %s\n",taskAlgoInfo->taskName.c_str());
		fflush(stdout);
		return false;
	}
	//
	// add autopilot task algo
	//
	mAutopilot[nAutopilotAlgo++]=apAlgo;
	return true;
}

//	//
//	// AUTO_HEADING - PI_autopilot
//	//
//	if(taskAlgoInfo->algoName == "PI_autopilot") {
//		if(taskAlgoInfo->taskName == AUTO_HEADING) {
//			// instantiate AUTO_HEADING - PI_autopilot
//			GuidanceControlAlgo* apAlgo = new GuidanceControlAlgo();
//			apAlgo->set_algo(taskAlgoInfo->algoName);
//			apAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				apAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add autopilot task algo
//			mAutopilot[nAutopilotAlgo++]=apAlgo;
//			return true;
//		}
//	}
//	//
//	// AUTO_COURSE - PI_autopilot
//	//
//	if(taskAlgoInfo->algoName == "PI_autopilot") {
//		if(taskAlgoInfo->taskName == AUTO_COURSE) {
//			// instantiate AUTO_COURSE - PI_autopilot
//			GuidanceControlAlgo* apAlgo = new GuidanceControlAlgo();
//			apAlgo->set_algo(taskAlgoInfo->algoName);
//			apAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				apAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add autopilot task algo
//			mAutopilot[nAutopilotAlgo++]=apAlgo;
//			return true;
//		}
//	}
//	printf("Autopilot::instantiate_task task %s algo %s is NOT processed!\n",taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
//	fflush(stdout);
//	return false;
//}


void Autopilot::process_command(char *cmd) {
//	printf("Autopilot::process_command : %s\r\n",cmd);fflush(stdout);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="AUTOPILOT") {
		if(mAutopilotCmd.decode(cmd, strCmd)) {
			if(strCmd=="SET_HEADING")
				process_set_heading();
			else if(strCmd=="SET_COURSE") {
				process_set_course();
			}
			else  {
				printf("Autopilot::decode ERROR read - Autopilot cmd is %s\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="AUTOPILOT")
	else  {
		printf("Autopilot::process_command ERROR read - type is %s\n",strType.c_str());
		fflush(stdout);
		return ;
	}

}


void Autopilot::process_set_heading(void) {
//	printf("Autopilot::process_set_heading %ld\r\n",(long int)mAutopilotRef);fflush(stdout);
	mAutopilotRef->psi=mAutopilotCmd.psi;
}


void Autopilot::process_set_course(void) {
//	printf("Autopilot::process_set_course\r\n");fflush(stdout);
	mAutopilotRef->chi=mAutopilotCmd.chi;
}


