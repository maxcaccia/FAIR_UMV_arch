/*
 * Guidance.cpp
 *
 *  Created on: 14 May 2024
 *      Author: massimo
 */

#include "Guidance.h"

Guidance::Guidance(void) {
	mName="";
    //
    // Guidance & Control Tasks references
    //
    mGuidanceRef=NULL;
    mAutopilotRef=NULL;
    mCntrlRef=NULL;
    //
    // robot kinematics estimates
    //
    mPositionHat=NULL;
    mAttitudeHat=NULL;
    mLinearRateHat=NULL;
    //
    // Guidance Algo
    //
    for(int i=0;i<N_MAX_ALGO;i++) {
    		mGuidance[i]=NULL;
    }
    nGuidanceAlgo=0;

    mActiveGuidance=NULL;

    //
    // build selected task-algorithm table
    //
    selected_guidance_algo[0][0]=PATH_FOLLOWING;
    selected_guidance_algo[1][0]=LINE_FOLLOWING;
    selected_guidance_algo[2][0]=GO_TO_HEADING;
    selected_guidance_algo[3][0]=GO_TO_COURSE;
    selected_guidance_algo[4][0]=KEEP_POINT;
    for(int i=0;i<N_GUIDANCE_TASK;i++)
    	selected_guidance_algo[i][1]="null";

}

void Guidance::reset(void) {
	// clean select task-algorithm table
	for(int i=0;i<N_GUIDANCE_TASK;i++)
	    selected_guidance_algo[i][1]="null";

	// clean active Guidance
	mActiveGuidance=NULL;

	// reset all Guidance algorithms
    for(int i=0;i<nGuidanceAlgo;i++) {
    		mGuidance[i]->reset();
    }
}

Guidance::~Guidance(void) {
	if(mGuidanceRef!=NULL) {
		mGuidanceRef=NULL;
	}
	if(mAutopilotRef!=NULL) {
		mAutopilotRef=NULL;
	}
	if(mCntrlRef!=NULL) {
		mCntrlRef=NULL;
	}
	if(mPositionHat!=NULL) {
		mPositionHat=NULL;
	}
	if(mAttitudeHat!=NULL) {
		mAttitudeHat=NULL;
	}
	if(mLinearRateHat!=NULL) {
		mLinearRateHat=NULL;
	}
}


bool Guidance::read_configuration(string filename) {
	FILE *f;
	TaskAlgoInfo taskAlgoInfo;
	bool flag=false;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("Guidance::read_configuration : ERROR opening file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}

	//
	// read header
	//
	string str;
	// read ======
	fread_string(f,str);
	// read guidance tag
	fread_string(f,str);
	if(str!="#guidance") {
		printf("Guidance::read_configuration ERROR - #guidance expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	// read guidance name
	fread_string(f,mName);
	// read ======
	fread_string(f,str);

	while(read_task_algo(f,&taskAlgoInfo,flag)) {
		// instantiate Guidance task algo
//		printf("Guidance::read_configuration - instantiate task %s algo %s\n",taskAlgoInfo.taskName.c_str(),taskAlgoInfo.algoName.c_str());
//		fflush(stdout);
		if(!instantiate_task(&taskAlgoInfo)) {
			fclose(f);
			return false;
		}

	}
	if(!flag) {
		printf("Guidance::read_configuration( %s ) failed!\n",filename.c_str());
		fflush(stdout);
		fclose(f);
		return false;
	}

	fclose(f);
	return true;
}


bool Guidance::read_default_algorithms(string filename) {
	FILE *f;
	TaskAlgoInfo taskAlgoInfo;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("Guidance::read_default_algorithms : ERROR opening file %s\n",filename.c_str());
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
	// read guidance tag
	fread_string(f,str);
	if(str!="#guidance") {
		printf("Guidance::read_default_algorithms ERROR - #control expected %s found\n",str.c_str());
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
		for(int i=0;i<nGuidanceAlgo;i++)
			flag=flag||((mGuidance[i]->get_task()==task)&&(mGuidance[i]->get_algo()==algo));
		if(flag) {
			// set default task algo
			printf("Guidance::read_default_algorithms %s %s\n",task.c_str(),algo.c_str());
			set_selected_algo(task,algo);
		}
		else {
			printf("Guidance::read_default_algorithms ERROR - (%s,%s) not instantiated\n",task.c_str(),algo.c_str());
			fclose(f);
			return false;
		}

	}

	fclose(f);
	return true;
}


GuidanceControlAlgo* Guidance::get(string task,string algo) {
	for(int i=0;i<nGuidanceAlgo;i++) {
		if((mGuidance[i]->get_task()==task)&&(mGuidance[i]->get_algo()==algo))
			return mGuidance[i];
	}

	return NULL;
}

void Guidance::set_selected_algo(string task,string algo) {
	for(int i=0;i<N_GUIDANCE_TASK;i++)
		if(selected_guidance_algo[i][0]==task)
			selected_guidance_algo[i][1]=algo;
}

string Guidance::get_selected_algo(string task) {
	for(int i=0;i<N_GUIDANCE_TASK;i++)
		if(selected_guidance_algo[i][0]==task)
			return selected_guidance_algo[i][1];
	printf("Guidance::get_selected_algo unexpected task %s\n",task.c_str());
	fflush(stdout);
	return("null");
}


void Guidance::set_task(string  task) {
	if(task=="null") {
		mActiveGuidance=NULL;
		return ;
	}
	// Guidance Task: "PF" / "LF" / "go_to_heading" / "go_to_course" / "keep_point" / "null"
	if((task==PATH_FOLLOWING)||(task==LINE_FOLLOWING)||(task==GO_TO_HEADING)||(task==GO_TO_COURSE)||(task==KEEP_POINT)) {
		string algo=get_selected_algo(task);
		if(algo!="null") {
			GuidanceControlAlgo* gcAlgo=get(task,algo);
			if(gcAlgo!=NULL) {
				mActiveGuidance=gcAlgo;
				mActiveGuidance->reset();
			}
			else {
				printf("Guidance::set_guidance_task unexpected task %s algo %s\n",task.c_str(),algo.c_str());
				fflush(stdout);
			}
		}
		else {
			printf("Guidance::set_guidance_task task %s selected algo is %s\n",task.c_str(),algo.c_str());
			fflush(stdout);
		}
	}
	else {
		printf("Guidance::set_guidance_task unexpected task %s\n",task.c_str());
		fflush(stdout);
	}
}



void Guidance::set_io_ref(GuidanceReference *guidanceRef,AutopilotReference *autopilotRef,ControlReference *cntrlRef) {
	mGuidanceRef=guidanceRef;
	mAutopilotRef=autopilotRef;
	mCntrlRef=cntrlRef;
}

void Guidance::set_estimates(LogicalPositionData *positionHat,LogicalAttitudeData *attitudeHat,LogicalLinearRateData *linearRateHat) {
	mPositionHat=positionHat;
	mAttitudeHat=attitudeHat;
	mLinearRateHat=linearRateHat;
}

void Guidance::execute(void) {
	printf("Guidance::execute\n");fflush(stdout);
	//
	// execute Guidance
	//
	if(mActiveGuidance!=NULL)
		mActiveGuidance->execute();
}


void Guidance::print(void) {
	printf("===================================\n");
	printf("===   Guidance\n");
	printf("===================================\n");
	//
	// print Guidance task-algo
	//
	for(int i=0;i<nGuidanceAlgo;i++)
		mGuidance[i]->print();
	fflush(stdout);
}

bool Guidance::instantiate_task(TaskAlgoInfo* taskAlgoInfo) {
//	printf("Guidance::instantiate_task %s %s\n",taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
//	fflush(stdout);
	//
	// instantiate algo:	PF_VT_yawrate , PF_VT_heading
	//						LF_VT_yawrate , LF_PD_yawrate
	//						LOS
	//						PI_3dofs
	//
	GuidanceControlAlgo* gAlgo=NULL;
	if(taskAlgoInfo->algoName == "PF_VT_yawrate") {
		if(taskAlgoInfo->taskName != PATH_FOLLOWING) {
			printf("Guidance::instantiate_task - ERROR - unexpected (task,algo) : (%s,%s)\n",
					taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
			fflush(stdout);
			return false;
		}
		// instantiate algo - PF_VT_yawrate
		gAlgo = new GuidanceControlAlgo();
	}
	else if(taskAlgoInfo->algoName == "PF_VT_heading") {
		if(taskAlgoInfo->taskName != PATH_FOLLOWING) {
			printf("Guidance::instantiate_task - ERROR - unexpected (task,algo) : (%s,%s)\n",
					taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
			fflush(stdout);
			return false;
		}
		// instantiate algo - PF_VT_heading
		gAlgo = new GuidanceControlAlgo();
	}
	else if(taskAlgoInfo->algoName == "LF_VT_yawrate") {
		if(taskAlgoInfo->taskName != LINE_FOLLOWING) {
			printf("Guidance::instantiate_task - ERROR - unexpected (task,algo) : (%s,%s)\n",
					taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
			fflush(stdout);
			return false;
		}
		// instantiate algo - LF_VT_yawrate
		gAlgo = new GuidanceControlAlgo();
	}
	else if(taskAlgoInfo->algoName == "LF_PD_yawrate") {
		if(taskAlgoInfo->taskName != LINE_FOLLOWING) {
			printf("Guidance::instantiate_task - ERROR - unexpected (task,algo) : (%s,%s)\n",
					taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
			fflush(stdout);
			return false;
		}
		// instantiate algo - LF_PD_yawrate
		gAlgo = new LineFollowingPD();
	}
	else if(taskAlgoInfo->algoName == "LOS") {
		if((taskAlgoInfo->taskName != GO_TO_HEADING) && (taskAlgoInfo->taskName != GO_TO_COURSE)) {
			printf("Guidance::instantiate_task - ERROR - unexpected (task,algo) : (%s,%s)\n",
					taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
			fflush(stdout);
			return false;
		}
		// instantiate algo - LOS
		gAlgo = new LineOfSight();
	}
	else if(taskAlgoInfo->algoName == "PI_3dofs") {
		if(taskAlgoInfo->taskName != KEEP_POINT) {
			printf("Guidance::instantiate_task - ERROR - unexpected (task,algo) : (%s,%s)\n",
					taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
			fflush(stdout);
			return false;
		}
		// instantiate algo - PI_3dofs
		gAlgo = new GuidanceControlAlgo();
	}
	else {
		printf("Guidance::instantiate_task - ERROR : unexpected algo %s\n",taskAlgoInfo->algoName.c_str());
		fflush(stdout);
		return false;
	}
	//
	// set name, task, algo
	//
	gAlgo->set_algo(taskAlgoInfo->algoName);
	gAlgo->set_task(taskAlgoInfo->taskName);
	gAlgo->set_name(taskAlgoInfo->taskName+"_"+taskAlgoInfo->algoName);
	// read algo configuration, i.e. parameters
	string s="GuidanceControl/";
	string filename=NGC_CONFIG_PATH+s+gAlgo->get_name()+".cfg";
	printf("Guidance::instantiate_task read_configuration %s\n",filename.c_str());fflush(stdout);
	if(!gAlgo->read_configuration(filename)) {
		printf("Guidance::instantiate_task error reading configuration file %s\n",filename.c_str());fflush(stdout);
		return false;
	}
	// set dependencies (reference)
	for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
		string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
		gAlgo->add_required_task(task);
	}
	// set dependencies (estimate): to be done
	//
	// set task : PATH_FOLLOWING , LINE_FOLLOWING , GO_TO_HEADING , GO_TO_COURSE , KEEP_POINT
	//			I/O variables
	//
	if(taskAlgoInfo->taskName == GO_TO_HEADING) {
		// set I/O variables
		gAlgo->set_input_output(&(mGuidanceRef->latWP), &(mPositionHat->lat),
				&(mGuidanceRef->lonWP), &(mPositionHat->lon),
				&(mGuidanceRef->uBar),
				&(mAutopilotRef->psi), &(mAutopilotRef->uBar));
	}
	else if(taskAlgoInfo->taskName == GO_TO_COURSE) {
		// set I/O variables
		gAlgo->set_input_output(&(mGuidanceRef->latWP), &(mPositionHat->lat),
				&(mGuidanceRef->lonWP), &(mPositionHat->lon),
				&(mGuidanceRef->uBar),
				&(mAutopilotRef->chi), &(mAutopilotRef->uBar));
	}
	else if((taskAlgoInfo->taskName == PATH_FOLLOWING) ||
			(taskAlgoInfo->taskName == LINE_FOLLOWING) ||
			(taskAlgoInfo->taskName == KEEP_POINT)) {
		printf("Guidance::instantiate_task - WARNING : task %s set I/O variables to be implemented!\n",taskAlgoInfo->taskName.c_str());
		fflush(stdout);
	}
	else {
		printf("Guidance::instantiate_task - ERROR : unexpected task %s\n",taskAlgoInfo->taskName.c_str());
		fflush(stdout);
		return false;
	}
	//
	// add guidance task algo
	//
	mGuidance[nGuidanceAlgo++]=gAlgo;
	return true;




//	//
//	// PATH_FOLLOWING - PF_VT_yawrate
//	//
//	if(taskAlgoInfo->algoName == "PF_VT_yawrate") {
//		if(taskAlgoInfo->taskName == PATH_FOLLOWING) {
//			// instantiate PATH_FOLLOWING - PF_VT_yawrate
//			GuidanceControlAlgo* gcAlgo = new GuidanceControlAlgo();
//			gcAlgo->set_algo(taskAlgoInfo->algoName);
//			gcAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				gcAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add guidance task algo
//			mGuidance[nGuidanceAlgo++]=gcAlgo;
//			return true;
//		}
//	}
//	//
//	// PATH_FOLLOWING - PF_VT_heading
//	//
//	if(taskAlgoInfo->algoName == "PF_VT_heading") {
//		if(taskAlgoInfo->taskName == PATH_FOLLOWING) {
//			// instantiate PATH_FOLLOWING - PF_VT_heading
//			GuidanceControlAlgo* gcAlgo = new GuidanceControlAlgo();
//			gcAlgo->set_algo(taskAlgoInfo->algoName);
//			gcAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				gcAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add guidance task algo
//			mGuidance[nGuidanceAlgo++]=gcAlgo;
//			return true;
//		}
//	}
//	//
//	// LINE_FOLLOWING - LF_VT_yawrate
//	//
//	if(taskAlgoInfo->algoName == "LF_VT_yawrate") {
//		if(taskAlgoInfo->taskName == LINE_FOLLOWING) {
//			// instantiate LINE_FOLLOWING - LF_VT_yawrate
//			GuidanceControlAlgo* gcAlgo = new GuidanceControlAlgo();
//			gcAlgo->set_algo(taskAlgoInfo->algoName);
//			gcAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				gcAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add guidance task algo
//			mGuidance[nGuidanceAlgo++]=gcAlgo;
//			return true;
//		}
//	}
//	//
//	// LINE_FOLLOWING - LF_PD_yawrate
//	//
//	if(taskAlgoInfo->algoName == "LF_PD_yawrate") {
//		if(taskAlgoInfo->taskName == LINE_FOLLOWING) {
//			// instantiate LINE_FOLLOWING - PF_VT_heading
//			GuidanceControlAlgo* gcAlgo = new GuidanceControlAlgo();
//			gcAlgo->set_algo(taskAlgoInfo->algoName);
//			gcAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				gcAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add guidance task algo
//			mGuidance[nGuidanceAlgo++]=gcAlgo;
//			return true;
//		}
//	}
//	//
//	// GO_TO_HEADING - LOS
//	//
//	if(taskAlgoInfo->algoName == "LOS") {
//		if(taskAlgoInfo->taskName == GO_TO_HEADING) {
//			// instantiate GO_TO_HEADING - LOS
//			GuidanceControlAlgo* gcAlgo = new GuidanceControlAlgo();
//			gcAlgo->set_algo(taskAlgoInfo->algoName);
//			gcAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				gcAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add guidance task algo
//			mGuidance[nGuidanceAlgo++]=gcAlgo;
//			return true;
//		}
//	}
//	//
//	// GO_TO_COURSE - LOS
//	//
//	if(taskAlgoInfo->algoName == "LOS") {
//		if(taskAlgoInfo->taskName == GO_TO_COURSE) {
//			// instantiate GO_TO_COURSE - LOS
//			GuidanceControlAlgo* gcAlgo = new GuidanceControlAlgo();
//			gcAlgo->set_algo(taskAlgoInfo->algoName);
//			gcAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				gcAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add guidance task algo
//			mGuidance[nGuidanceAlgo++]=gcAlgo;
//			return true;
//		}
//	}
//	//
//	// KEEP_POINT - PI_3dofs
//	if(taskAlgoInfo->algoName == "PI_3dofs") {
//		if(taskAlgoInfo->taskName == KEEP_POINT) {
//			// instantiate KEEP_POINT - PI_3dofs
//			GuidanceControlAlgo* gcAlgo = new GuidanceControlAlgo();
//			gcAlgo->set_algo(taskAlgoInfo->algoName);
//			gcAlgo->set_task(taskAlgoInfo->taskName);
//			// set dependencies (reference)
//			for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
//				string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
//				gcAlgo->add_required_task(task);
//			}
//			// set dependencies (estimate): to be done
//			// set I/O variables : to be done
//			// add guidance task algo
//			mGuidance[nGuidanceAlgo++]=gcAlgo;
//			return true;
//		}
//	}
//	printf("Guidance::instantiate_task task %s algo %s is NOT processed!\n",taskAlgoInfo->taskName.c_str(),taskAlgoInfo->algoName.c_str());
//	fflush(stdout);
//	return false;
}


void Guidance::process_command(char *cmd) {
	//	printf("******************** %s ---- Guidance::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="GUIDANCE") {
		if(mGuidanceCmd.decode(cmd, strCmd)) {
			if(strCmd=="SET_KEEP_POINT")
				process_set_keep_point();
			else if(strCmd=="SET_WAYPOINT") {
				process_set_waypoint();
			}
			else if(strCmd=="SET_LINE") {
				process_set_line();
			}
			else if(strCmd=="SET_PATH") {
				process_set_path();
			}
			else  {
				printf("Guidance::decode ERROR read - Guidance cmd is %s\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="GUIDANCE")
	else  {
		printf("Guidance::process_command ERROR read - type is %s\n",strType.c_str());
		fflush(stdout);
		return ;
	}
}


void Guidance::process_set_keep_point(void) {
	mGuidanceRef->latWP=mGuidanceCmd.latWP;
	mGuidanceRef->lonWP=mGuidanceCmd.lonWP;
	mGuidanceRef->psiWP=mGuidanceCmd.psiWP;
}


void Guidance::process_set_waypoint(void) {
	mGuidanceRef->latWP=mGuidanceCmd.latWP;
	mGuidanceRef->lonWP=mGuidanceCmd.lonWP;
}


void Guidance::process_set_line(void) {
	mGuidanceRef->latL=mGuidanceCmd.latL;
	mGuidanceRef->lonL=mGuidanceCmd.lonL;
	mGuidanceRef->gammaL=mGuidanceCmd.gammaL;
}


void Guidance::process_set_path(void) {
	printf("Guidance::process_set_path(void) to be implemented!\n");
	fflush(stdout);
}


