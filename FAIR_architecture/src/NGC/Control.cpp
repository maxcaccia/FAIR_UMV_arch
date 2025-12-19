/*
 * Control.cpp
 *
 *  Created on: 14 May 2024
 *      Author: massimo
 */

#include "Control.h"

Control::Control(void) {
	mName="";
    //
    // Control & Force/Torque Tasks references
    //
    mCntrlRef=NULL;
    mForceMomentRef=NULL;
    //
    // robot kinematics estimates
    //
    mLinearRateHat=NULL; // linear velocity
    mAngularRateHat=NULL; // angular velocity

    mHydroPar=NULL;

    //
    // Guidance Algo
    //
    for(int i=0;i<N_MAX_ALGO;i++) {
    		mControl[i]=NULL;
    }
    nControlAlgo=0;

    mActiveAutoSurge=NULL;
    mActiveAutoSway=NULL;
    mActiveAutoYaw=NULL;
    //
    // build selected task-algorithm table
    //
    selected_control_algo[0][0]=AUTO_SURGE;
    selected_control_algo[1][0]=AUTO_SWAY;
    selected_control_algo[2][0]=AUTO_YAW;
    for(int i=0;i<N_CONTROL_TASK;i++)
    	selected_control_algo[i][1]="null";

}

void Control::reset(void) {
	// clean select task-algorithm table
	for(int i=0;i<N_CONTROL_TASK;i++)
	    selected_control_algo[i][1]="null";

	// clean active Control
    mActiveAutoSurge=NULL;
    mActiveAutoSway=NULL;
    mActiveAutoYaw=NULL;

	// reset all Control algorithms
    for(int i=0;i<nControlAlgo;i++) {
    		mControl[i]->reset();
    }
}

Control::~Control(void) {
	if(mCntrlRef!=NULL) {
		mCntrlRef=NULL;
	}
	if(mForceMomentRef!=NULL) {
		mForceMomentRef=NULL;
	}
	if(mLinearRateHat!=NULL) {
		mLinearRateHat=NULL;
	}
	if(mAngularRateHat!=NULL) {
		mAngularRateHat=NULL;
	}
}


bool Control::read_configuration(string filename) {
	FILE *f;
	TaskAlgoInfo taskAlgoInfo;
	bool flag=false;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("Control::read_configuration : ERROR opening file %s\n",filename.c_str());
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
	if(str!="#control") {
		printf("Control::read_configuration ERROR - #control expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	// read control name
	fread_string(f,mName);
	// read ======
	fread_string(f,str);

	while(read_task_algo(f,&taskAlgoInfo,flag)) {
		// instantiate Control task algo
		printf("Control::read_configuration - instantiate task %s algo %s\n",taskAlgoInfo.taskName.c_str(),taskAlgoInfo.algoName.c_str());
//		fflush(stdout);
		if(!instantiate_task(&taskAlgoInfo)) {
			printf("Control::read_configuration - instantiate task failed %s algo %s\n",taskAlgoInfo.taskName.c_str(),taskAlgoInfo.algoName.c_str());
			fclose(f);
			return false;
		}
	}
	if(!flag) {
		printf("Control::read_configuration( %s ) failed!\n",filename.c_str());
		fflush(stdout);
		fclose(f);
		return false;
	}

	fclose(f);
	return true;
}


bool Control::read_default_algorithms(string filename) {
	FILE *f;
	TaskAlgoInfo taskAlgoInfo;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("Control::read_default_algorithms : ERROR opening file %s\n",filename.c_str());
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
	if(str!="#control") {
		printf("Control::read_default_algorithms ERROR - #control expected %s found\n",str.c_str());
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
		for(int i=0;i<nControlAlgo;i++)
			flag=flag||((mControl[i]->get_task()==task)&&(mControl[i]->get_algo()==algo));
		if(flag) {
			// set default task algo
			printf("Control::read_default_algorithms %s %s\n",task.c_str(),algo.c_str());
			set_selected_algo(task,algo);
		}
		else {
			printf("Control::read_default_algorithms ERROR - (%s,%s) not instantiated\n",task.c_str(),algo.c_str());
			fclose(f);
			return false;
		}

	}

	fclose(f);
	return true;
}



GuidanceControlAlgo* Control::get(string task,string algo) {
	for(int i=0;i<nControlAlgo;i++) {
		if((mControl[i]->get_task()==task)&&(mControl[i]->get_algo()==algo))
			return mControl[i];
	}

	return NULL;
}

void Control::set_selected_algo(string task,string algo) {
	for(int i=0;i<N_CONTROL_TASK;i++)
		if(selected_control_algo[i][0]==task)
			selected_control_algo[i][1]=algo;

	print_selected_algo();
}

string Control::get_selected_algo(string task) {
	for(int i=0;i<N_CONTROL_TASK;i++)
		if(selected_control_algo[i][0]==task)
			return selected_control_algo[i][1];
	printf("Control::get_selected_algo unexpected task %s\n",task.c_str());
	fflush(stdout);
	return("null");
}


bool Control::enable_autosurge_task(void) {
	string algo=get_selected_algo(AUTO_SURGE);
	if(algo=="null") {
		mActiveAutoSurge=NULL;
		printf("Control::enable_autosurge_task task %s selected algo is %s\n",AUTO_SURGE,algo.c_str());
		fflush(stdout);
		return false;
	}
	else {
		GuidanceControlAlgo* gcAlgo=get(AUTO_SURGE,algo);
		if(gcAlgo!=NULL) {
			mActiveAutoSurge=gcAlgo;
			mActiveAutoSurge->reset();
			return true;
		}
		else {
			printf("Control::enable_autosurge_task unexpected task %s algo %s\n",AUTO_SURGE,algo.c_str());
			fflush(stdout);
			return false;
		}
	}
}

bool Control::enable_autosway_task(void) {
	string algo=get_selected_algo(AUTO_SWAY);
	if(algo=="null") {
		mActiveAutoSway=NULL;
		printf("Control::enable_autosway_task task %s selected algo is %s\n",AUTO_SWAY,algo.c_str());
		fflush(stdout);
		return false;
	}
	else {
		GuidanceControlAlgo* gcAlgo=get(AUTO_SWAY,algo);
		if(gcAlgo!=NULL) {
			mActiveAutoSway=gcAlgo;
			mActiveAutoSway->reset();
			return true;
		}
		else {
			printf("Control::enable_autosway_task unexpected task %s algo %s\n",AUTO_SWAY,algo.c_str());
			fflush(stdout);
			return false;
		}
	}
}

bool Control::enable_autoyaw_task(void) {
	string algo=get_selected_algo(AUTO_YAW);
	if(algo=="null") {
		mActiveAutoYaw=NULL;
		printf("Control::enable_autoyaw_task task %s selected algo is %s\n",AUTO_YAW,algo.c_str());
		fflush(stdout);
		return false;
	}
	else {
		GuidanceControlAlgo* gcAlgo=get(AUTO_YAW,algo);
		if(gcAlgo!=NULL) {
			mActiveAutoYaw=gcAlgo;
			mActiveAutoYaw->reset();
			return true;
		}
		else {
			printf("Control::enable_autoyaw_task unexpected task %s algo %s\n",AUTO_YAW,algo.c_str());
			fflush(stdout);
			return false;
		}
	}
}


void Control::set_io_ref(ControlReference *cntrlRef,ForceMoment *forceMomentRef) {
	mCntrlRef=cntrlRef;
	mForceMomentRef=forceMomentRef;
}

void Control::set_estimates(LogicalLinearRateData *linearRateHat,LogicalAngularRateData *angularRateHat) {
	mLinearRateHat=linearRateHat;
	mAngularRateHat=angularRateHat;
}


//bool Control::is_active(string task) {
//	if(task==AUTO_SURGE) return (mActiveAutoSurge!=NULL);
//	if(task==AUTO_SWAY) return (mActiveAutoSway!=NULL);
//	if(task==AUTO_YAW) return (mActiveAutoYaw!=NULL);
//	return false;
//}


void Control::execute(void) {
	printf("Control::execute\n");fflush(stdout);
	//
	// execute Control
	//
	if(mActiveAutoSurge!=NULL)
		mActiveAutoSurge->execute();
	if(mActiveAutoSway!=NULL)
		mActiveAutoSway->execute();
	if(mActiveAutoYaw!=NULL)
		mActiveAutoYaw->execute();
}



void Control::print(void) {
	printf("===================================\n");
	printf("===   Control\n");
	printf("===================================\n");
	//
	// print Control task-algo
	//
	for(int i=0;i<nControlAlgo;i++)
		mControl[i]->print();
	fflush(stdout);
}

void Control::print_selected_algo(void) {
	printf("Control::print_selected_algo\n");
	for(int i=0;i<N_CONTROL_TASK;i++)
		printf("%s %s\n",selected_control_algo[i][0].c_str(),selected_control_algo[i][1].c_str());
}

bool Control::instantiate_task(TaskAlgoInfo* taskAlgoInfo) {
	//
	// instantiate algo: PI_GS , PI_GS_yaw
	//
	GuidanceControlAlgo* cAlgo=NULL;
	if(taskAlgoInfo->algoName == "PI_GS") {
		// instantiate algo - PI_GS
		cAlgo = new GainSchedPI(NGC_DELTA_T);
	}
	else if(taskAlgoInfo->algoName == "PI_GS_yaw") {
		// instantiate algo - PI_GS
		cAlgo = new GainSchedPIyaw(NGC_DELTA_T);
	}
	else {
		printf("Control::instantiate_task - ERROR : unexpected algo %s\n",taskAlgoInfo->algoName.c_str());
		fflush(stdout);
		return false;
	}
	//
	// set name, task, algo
	//
	cAlgo->set_algo(taskAlgoInfo->algoName);
	cAlgo->set_task(taskAlgoInfo->taskName);
	cAlgo->set_name(taskAlgoInfo->taskName+"_"+taskAlgoInfo->algoName);
	// read algo configuration, i.e. parameters
	string s="GuidanceControl/";
	string filename=NGC_CONFIG_PATH+s+cAlgo->get_name()+".cfg";
	printf("Control::instantiate_task read_configuration %s\n",filename.c_str());fflush(stdout);
	if(!cAlgo->read_configuration(filename)) {
		printf("Control::instantiate_task error reading configuration file %s\n",filename.c_str());fflush(stdout);
		return false;
	}
	// set dependencies (reference)
	for(int i=0;i<taskAlgoInfo->nReferenceOutput;i++) {
		string task=get_required_task(taskAlgoInfo->referenceOutput[i]);
		cAlgo->add_required_task(task);
	}
	// set dependencies (estimate): to be done
	//
	// set task : AUTO_SURGE , AUTO_SWAY , AUTO_YAW
	//			I/O variables and hydrodynamics parameters
	//
	if(taskAlgoInfo->taskName == AUTO_SURGE) {
		// set I/O variables
		cAlgo->set_input_output(&(mCntrlRef->u), &(mLinearRateHat->u), &(mForceMomentRef->X));
		// set hydrodynamics parameters
		((GainSchedPI*)cAlgo)->set_hydrodynamics_parameters(mHydroPar->mu, mHydroPar->Xu, mHydroPar->Xuu);
	}
	else if(taskAlgoInfo->taskName == AUTO_SWAY) {
		// set I/O variables
		cAlgo->set_input_output(&(mCntrlRef->v), &(mLinearRateHat->v), &(mForceMomentRef->Y));
		// set hydrodynamics parameters
		((GainSchedPI*)cAlgo)->set_hydrodynamics_parameters(mHydroPar->mv, mHydroPar->Yv, mHydroPar->Yvv);
	}
	else if(taskAlgoInfo->taskName == AUTO_YAW) {
		// set I/O variables
		cAlgo->set_input_output(&(mCntrlRef->r), &(mAngularRateHat->r), &(mForceMomentRef->N),  &(mLinearRateHat->u));
		// set hydrodynamics parameters
		((GainSchedPIyaw*)cAlgo)->set_hydrodynamics_parameters(mHydroPar->Ir, mHydroPar->Nr, mHydroPar->Nrr, mHydroPar->m*mHydroPar->xg);
	}
	else {
		printf("Control::instantiate_task - ERROR : unexpected task %s\n",taskAlgoInfo->taskName.c_str());
		fflush(stdout);
		return false;
	}
	//
	// add control task algo
	//
	mControl[nControlAlgo++]=cAlgo;
	return true;
}


void Control::process_command(char *cmd) {
//	printf("******************** %s ---- Control::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="CNTRL") {
		if(mCntrlCmd.decode(cmd, strCmd)) {
			if(strCmd=="SET_SURGE")
				process_set_surge();
			else if(strCmd=="SET_SWAY") {
				process_set_sway();
			}
			else if(strCmd=="SET_YAW") {
				process_set_yaw();
			}
			else  {
				printf("Control::decode ERROR read - Control cmd is %s\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="CNTRL")
	else  {
		printf("Control::process_command ERROR read - type is %s\n",strType.c_str());
		fflush(stdout);
		return ;
	}

}


void Control::process_set_surge(void) {
	mCntrlRef->u=mCntrlCmd.u;
}


void Control::process_set_sway(void) {
	mCntrlRef->v=mCntrlCmd.v;
}


void Control::process_set_yaw(void) {
	mCntrlRef->r=mCntrlCmd.r;
}
