/*
 * NGC.cpp
 *
 *  Created on: 17 May 2024
 *      Author: massimo
 */

#include "NGC.h"

NGC::NGC(void) {
	mName="";
	mConfigDir="";
    //
    // Guidance, Autopilot & Control Tasks references
    //
    mGuidanceRef=NULL;
    mAutopilotRef=NULL;
    mCntrlRef=NULL;
    //
    // robot kinematics estimates
    //
    mPositionHat=NULL;
    mAttitudeHat=NULL;
    mSpeedHat=NULL;
    mLinearRateHat=NULL;
    mLinearRateWrtWaterHat=NULL;
    mAngularRateHat=NULL;

    //
    // logical sensor measurements
    //
    mPositionData=NULL;
    mAttitudeData=NULL;
    mSpeedData=NULL;
    mLinearRateData=NULL;
    mAngularRateData=NULL;

    //
    // TCA interface: robot force and torque
    //
    mForceMomentHat=NULL; // estimated force and moment
    mForceMomentRef=NULL; // referenmomentce force and torque
    //
    // robot hydrodynamics
    //
    mHydroPar=NULL;
    //
    // NGC: local NED frame latitude and longitude
    //
    mLatitudeNED0=NULL;
    mLongitudeNED0=NULL;

    mTimeStamp=NULL;
}

NGC::~NGC(void) {
	if(mGuidanceRef!=NULL) {
		mGuidanceRef=NULL;
	}
	if(mAutopilotRef!=NULL) {
		mAutopilotRef=NULL;
	}
	if(mCntrlRef!=NULL) {
		mCntrlRef=NULL;
	}
	if(mPositionData!=NULL) {
		mPositionData=NULL;
	}
	if(mAttitudeData!=NULL) {
		mAttitudeData=NULL;
	}
	if(mSpeedData!=NULL) {
		mSpeedData=NULL;
	}
	if(mLinearRateData!=NULL) {
		mLinearRateData=NULL;
	}
	if(mAngularRateData!=NULL) {
		mAngularRateData=NULL;
	}

	if(mPositionHat!=NULL) {
		mPositionHat=NULL;
	}
	if(mAttitudeHat!=NULL) {
		mAttitudeHat=NULL;
	}
	if(mSpeedHat!=NULL) {
		mSpeedHat=NULL;
	}
	if(mLinearRateHat!=NULL) {
		mLinearRateHat=NULL;
	}
	if(mAngularRateHat!=NULL) {
		mAngularRateHat=NULL;
	}
	if(mForceMomentRef!=NULL) {
		mForceMomentRef=NULL;
	}
	if(mForceMomentHat!=NULL) {
		mForceMomentHat=NULL;
	}
	if(mHydroPar!=NULL) {
		mHydroPar=NULL;
	}
}


bool NGC::configure_modules(void) {
	//
	// configure module I/Os
	//
	// Execution Control
	mExecutionControl.init(&mGuidance, &mAutopilot, &mControl);
	// Guidance
	mGuidance.set_io_ref(mGuidanceRef, mAutopilotRef, mCntrlRef);
	mGuidance.set_estimates(mPositionHat, mAttitudeHat, mLinearRateHat);
	// Autopilot
	mAutopilot.set_io_ref(mAutopilotRef, mCntrlRef);
	mAutopilot.set_estimates(mAttitudeHat, mSpeedHat);
	// Control
	mControl.set_io_ref(mCntrlRef, mForceMomentRef);
	mControl.set_estimates(mLinearRateHat, mAngularRateHat);
	mControl.set_hydrodynamics(mHydroPar);
	// Navigation
	mNavigation.set_control_actions(mForceMomentHat);
	mNavigation.set_measurements(mPositionData, mAttitudeData, mSpeedData, mLinearRateData, mAngularRateData);
	mNavigation.set_estimates(mPositionHat, mAttitudeHat, mSpeedHat, mLinearRateHat, mLinearRateWrtWaterHat, mAngularRateHat, mSeaCurrentHat);
	mNavigation.set_hydrodynamics(mHydroPar);
	mNavigation.set_time_stamp(mTimeStamp);
	//
	// read local NED frame
	//
	string filename;
	filename=mConfigDir+"localNEDframeNGC.cfg";
	if(!fread_local_NED_frame(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	//
	// read hydrodynamics model
	//
	filename=mConfigDir+"hydrodynamicsNGC.cfg";
	if(!mHydroPar->fread(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	//
	// read module configurations
	//
	// Guidance
	filename=mConfigDir+"guidance.cfg";
	if(!mGuidance.read_configuration(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	filename=mConfigDir+"guidance.default.cfg";
	if(!mGuidance.read_default_algorithms(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	// Autopilot
	filename=mConfigDir+"autopilot.cfg";
	if(!mAutopilot.read_configuration(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	filename=mConfigDir+"autopilot.default.cfg";
	if(!mAutopilot.read_default_algorithms(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	// Control
	filename=mConfigDir+"control.cfg";
	if(!mControl.read_configuration(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	filename=mConfigDir+"control.default.cfg";
	if(!mControl.read_default_algorithms(filename)) {
		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}
	//
	// instantiate Navigation
	//
//	// read sensor allocation
//	mGnssBodyInfo->reset();
//	mGnssSternInfo->reset();
//	mGnssBowInfo->reset();
//	mAhrsBodyInfo->reset();
//	filename=mConfigDir+"sensors.allocation.cfg";
//	if(!mNavigation.read_sensor_allocation(filename)) {
//		printf("NGC::configure_modules ERROR reading file %s\n",filename.c_str());
//		fflush(stdout);
//		return false;
//	}
	// instantiate tasks
	if(!mNavigation.instantiate_tasks()) {
		printf("NGC::configure_modules ERROR executing file mNavigation.instantiate_tasks()\n");
		fflush(stdout);
		return false;
	}


	mNavigation.print();
	mGuidance.print();
	mAutopilot.print();
	mControl.print();

	return true;
}


bool NGC::fread_local_NED_frame(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("ERROR: bool NGC::fread_local_NED_frame(string filename) - %s not found!\n",filename.c_str());
		return false;
	}

	char str[256];
	// latitude NED 0
	fscanf(f,"%s %lf",str,mLatitudeNED0);
    if(strcmp(str,"latitudeNED0")!=0)
    {
        printf("NGC::fread_local_NED_frame latitudeNED0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// longitude NED 0
	fscanf(f,"%s %lf",str,mLongitudeNED0);
    if(strcmp(str,"longitudeNED0")!=0)
    {
        printf("NGC::fread_local_NED_frame longitudeNED0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

    fclose(f);
	return true;
}


void NGC::execute(void) {
	printf("NGC::execute()\n"); fflush(stdout);

	mNavigation.execute();
	mGuidance.execute();
	mAutopilot.execute();
	mControl.execute();

//	print();
//	mControl.print_selected_algo();

//	printf_results();
}


void NGC::process_command(char *cmd) {
printf("NGC::process_command()\n"); fflush(stdout);
	char chrType[64];
	string strType;
	sscanf(cmd,"%s",chrType);
	strType=chrType;

	if(strType=="CNTRL")
		mControl.process_command(cmd);
	else if(strType=="AUTOPILOT")
		mAutopilot.process_command(cmd);
	else if(strType=="GUIDANCE")
		mGuidance.process_command(cmd);
	else if(strType=="EXEC_CNTRL")
		mExecutionControl.process_command(cmd);
	else {
		printf("NGC::process_command ERROR read - type is %s\n",strType.c_str());
		fflush(stdout);
		return ;
	}
}


void NGC::process_data(char *data,ssize_t nRecv) {
	mNavigation.process_data(data, nRecv);
}


void NGC::print(void) {
	string activeGuidanceTask;
	string activeGuidanceAlgo;
	string activeAutopilotTask;
	string activeAutopilotAlgo;
	string activeSurgeControlTask;
	string activeSurgeControlAlgo;
	string activeSwayControlTask;
	string activeSwayControlAlgo;
	string activeYawControlTask;
	string activeYawControlAlgo;
	GuidanceControlAlgo *ptr;

	ptr=mGuidance.get_active_guidance();
	if(ptr!=NULL) activeGuidanceTask=ptr->get_task().c_str();
	else activeGuidanceTask="none";
	if(ptr!=NULL) activeGuidanceAlgo=ptr->get_algo().c_str();
	else activeGuidanceAlgo="none";

	ptr=mAutopilot.get_active_autopilot();
	if(ptr!=NULL) activeAutopilotTask=ptr->get_task().c_str();
	else activeAutopilotTask="none";
	if(ptr!=NULL) activeAutopilotAlgo=ptr->get_algo().c_str();
	else activeAutopilotAlgo="none";

	ptr=mControl.get_active_autosurge();
	if(ptr!=NULL) activeSurgeControlTask=ptr->get_task().c_str();
	else activeSurgeControlTask="none";
	if(ptr!=NULL) activeSurgeControlAlgo=ptr->get_algo().c_str();
	else activeSurgeControlAlgo="none";

	ptr=mControl.get_active_autosway();
	if(ptr!=NULL) activeSwayControlTask=ptr->get_task().c_str();
	else activeSwayControlTask="none";
	if(ptr!=NULL) activeSwayControlAlgo=ptr->get_algo().c_str();
	else activeSwayControlAlgo="none";

	ptr=mControl.get_active_autoyaw();
	if(ptr!=NULL) activeYawControlTask=ptr->get_task().c_str();
	else activeYawControlTask="none";
	if(ptr!=NULL) activeYawControlAlgo=ptr->get_algo().c_str();
	else activeYawControlAlgo="none";

	printf("NGC GUIDANCE task %s algo %s AUTOPILOT task %s algo %s "
			"CONTROL surge_task %s surge_algo %s sway_task %s sway_algo %s yaw_task %s yaw_algo %s\n",
			activeGuidanceTask.c_str(),activeGuidanceAlgo.c_str(),activeAutopilotTask.c_str(),activeAutopilotAlgo.c_str(),
			activeSurgeControlTask.c_str(),activeSurgeControlAlgo.c_str(),
			activeSwayControlTask.c_str(),activeSwayControlAlgo.c_str(),
			activeYawControlTask.c_str(),activeYawControlAlgo.c_str());
}


