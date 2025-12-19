/*
 * LPRIthread.cpp
 *
 *  Created on: Jul 11, 2025
 *      Author: mc
 */

#include "LPRIthread.h"

LPRIthread::LPRIthread(void): CntrlSimThread() {
	//printf("LPRIthread::LPRIthread begin %s %d %d\n",mName.c_str(),mPolicy,mPriority);fflush(stdout);
	mTCA=NULL; // mTCA is instantiated in the child class <Robot>TCAthread

//	mTCA = new TCA();

	mP2LSM = new P2LSM();

//	printf("LPRIthread::LPRIthread end %s %d %d\n",mName.c_str(),mPolicy,mPriority);fflush(stdout);
}

LPRIthread::~LPRIthread() {
	// TODO Auto-generated destructor stub
	if(mTCA!=NULL) {
		delete mTCA;
		mTCA=NULL;
	}
}

void LPRIthread::make_thread()
{
//    printf("TCAthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&LPRIthread::execute,this);
}

bool LPRIthread::init_configuration() {
//	printf("LPRIthread::init_configuration %s\n",mName.c_str());fflush(stdout);

	//////////////////////
	//
	// TCA
	//
	mXYNref.set_name("ref_"+mName);
	mXYNnormRef.set_name("ref_norm_"+mName);
	mXYNhat.set_name("est_"+mName);
	mXYNnormHat.set_name("est_norm_"+mName);

	mTCA->set_input_output(&mXYNref,&mXYNnormRef,&mXYNhat,&mXYNnormHat);

	mTCAstate.set_name("state_"+mName);
	mTCA->set_state(&mTCAstate);
//	mTCA->reset();

//	//
//	// to be removed
//	//
//	mTCA->set_propulsion_class("azimuth_jets_4");
	//
	// set manual mode
	//
	mTCA->set_mode("manual");

    //
    // TCA parameters
    //
	mTCA->set_name(mName);
	mTCA->set_config_dir(mThreadProperties.configDir);
	if(!mTCA->read_configuration()) {
//		printf("LPRIthread::init_configuration TCA failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		printf("LPRIthread::init_configuration mTCA->read_configuration() failed! %s\n",mName.c_str());fflush(stdout);
		return false;
	}
//	string fileName;
//    fileName=mThreadProperties.configDir+mName+"-TCAparam.cfg";
//	if(!mTCA->fread(fileName)) {
//		printf("LPRIthread::init_configuration TCA failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
//		return false;
//	}


	//////////////////////
	//
	// P2LSM
	//

	mP2LSM->set_name(mName);
	mP2LSM->set_config_dir(mThreadProperties.configDir+"P2LSM/");
	
	mLogicalPositionData.set_name("est_"+mName);
    mLogicalAttitudeData.set_name("est_"+mName);;
    mLogicalSpeedData.set_name("est_"+mName);;
    mLogicalLinearRateData.set_name("est_"+mName);;
    mLogicalAngularRateData.set_name("est_"+mName);;

//	mP2LSM->set_sensor_info(mSensorInfo,&nSensor);
	mP2LSM->set_sensor_info(&mSensorInfo);
	mP2LSM->set_driver_interface(&mAhrsData,&mGnssPositionData,&mGnssVelocityData,&mDvlData);
	mP2LSM->set_sensor_priorities(&mLogicalPosition,&mLogicalAttitude,
			&mLogicalSpeed,&mLogicalLinearRate,&mLogicalAngularRate);
	mP2LSM->set_ngc_interface(&mLogicalPositionData,&mLogicalAttitudeData,&mLogicalSpeedData,
			&mLogicalLinearRateData,&mLogicalAngularRateData);
	mP2LSMstate.set_name("state_"+mName);
	mP2LSM->set_state(&mP2LSMstate);
	mP2LSM->set_time_stamp(&mTimeStamp);

	mP2LSM->set_dt((double)(mThreadProperties.timeValue)/1000.);

	//
	// initialise Logical Sensors: Physical Sensor priorities & sensor information
	//
	if(!mP2LSM->read_configuration()) {
//		printf("LPRIthread::init_configuration TCA failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		printf("LPRIthread::init_configuration mP2LSM->read_configuration() failed! %s\n",mName.c_str());fflush(stdout);
		return false;
	}

	mP2LSM->reset_state();

	return true;
}

void LPRIthread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- LPRIthread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	mTCA->clear_nop();

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if((strType=="TCA")||(strType=="BLUEBOAT_TCA")||(strType=="SWAMP_TCA")) {
		mTCA->process_command(cmd, n);
	}
	else if(strType=="P2LSM") {
		mP2LSM->process_command(cmd, n);
	}
	//else if((strType=="GENERIC")&&(strCmd=="NOP")) {
		//printf("LPRIthread::process_command %s\n",cmd);
////		mTCA->clear_nop();
	//}

	else  {
//		printf("TCAthread::process_command ERROR read - type is %s\n",strType.c_str());
//		fflush(stdout);
		return ;
	}

}


void LPRIthread::process_data(char *data,ssize_t nRecv) {
//	printf("LPRIthread::process_data: %s data %s",mName.c_str(),data);

	bool flagTCA,flagP2LSM;

	flagTCA = mTCA->process_data(data, nRecv);
	flagP2LSM = mP2LSM->process_data(data,nRecv);

	if(!(flagTCA||flagP2LSM)) {
		//printf("LPRIthread::process_data unexpected data %s\n",data);
		//fflush(stdout);
	}
}


void LPRIthread::build_data(void) {
	Packet packet;
	/////////////////////////////
	//
	// TCA: append output to mDataSendBuffer
	//
	packet.n=mXYNref.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mXYNhat.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mXYNnormRef.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mXYNnormHat.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mTCAstate.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	/////////////////////////////////
	//
	// append output to mDataSendBuffer
	//
	if(mLogicalPositionData.mUpdated && mLogicalPositionData.mValid) {
		packet.n=mLogicalPositionData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalPositionData.mUpdated = mLogicalPositionData.mValid = false;
//		printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalAttitudeData.mUpdated && mLogicalAttitudeData.mValid) {
		packet.n=mLogicalAttitudeData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalAttitudeData.mUpdated = mLogicalAttitudeData.mValid = false;
//		printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalSpeedData.mUpdated && mLogicalSpeedData.mValid) {
		packet.n=mLogicalSpeedData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalSpeedData.mUpdated = mLogicalSpeedData.mValid = false;
//		printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalLinearRateData.mUpdated && mLogicalLinearRateData.mValid) {
		packet.n=mLogicalLinearRateData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalLinearRateData.mUpdated = mLogicalLinearRateData.mValid = false;
//		printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalAngularRateData.mUpdated && mLogicalAngularRateData.mValid) {
		packet.n=mLogicalAngularRateData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalAngularRateData.mUpdated = mLogicalAngularRateData.mValid = false;
//		printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	packet.n=mP2LSMstate.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("LPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

}

