/*
 * BlueBoatLPRIthread.cpp
 *
 *  Created on: Jul 28, 2025
 *      Author: mc
 */

#include "BlueBoatLPRIthread.h"
#include "BlueBoatTCA.h"

BlueBoatLPRIthread::BlueBoatLPRIthread(void): LPRIthread() {
//	mEnableRpmAzimuth=false;
	mTCA = new BlueBoatTCA();
}

BlueBoatLPRIthread::~BlueBoatLPRIthread() {
	// TODO Auto-generated destructor stub
	if(mTCA!=NULL) {
		delete mTCA;
		mTCA=NULL;
	}
}

void BlueBoatLPRIthread::make_thread()
{
//    printf("BlueBoatTCAthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&BlueBoatLPRIthread::execute,this);
}

bool BlueBoatLPRIthread::init_configuration(void) {
//printf("BlueBoatTCAthread::init_configuration %s\n",mName.c_str());fflush(stdout);
	//
	mTCA->set_name(mName);
	mP2LSM->set_name(mName);

	if(!LPRIthread::init_configuration()) {
		printf("BlueBoatLPRIthread::LPRIthread::init_configuration failed! %s\n",mName.c_str());fflush(stdout);
		return false;;
	}

	mBlueBoatRpmRef.set_name("ref_"+mName);
	mBlueBoatRpmMeas.set_name("msr_"+mName);

	((BlueBoatTCA*)mTCA)->set_input_output(&mBlueBoatRpmRef,&mBlueBoatRpmMeas);

	((BlueBoatTCA*)mTCA)->set_cmd_send_buffer(mCmdSendBuffer);

	mTCA->set_propulsion_class("differential_propellers");

	mTCA->reset();

	mTCA->set_manual_default();


	return true;
}


//void BlueBoatLPRIthread::process_command(char *cmd,ssize_t n) {
//	TCAthread::process_command(cmd,n);
//
//	printf("******************** %s ---- BlueBoatTCAthread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
//
//	char chrType[64],chrCmd[64];
//	string strType,strCmd;
//	sscanf(cmd,"%s %s",chrType,chrCmd);
//	strType=chrType;
//	strCmd=chrCmd;
//
//	if(strType=="BLUEBOAT_TCA") {
//		if(mBlueBoatTcaCmd.decode(cmd, strCmd)) {
//			if(strCmd=="SET_LEFT_RIGHT_RPM") {
//				mBlueBoatRpmRef.mTimeStamp = mTimeStamp;
//				process_set_left_right_rpm();
//			}
//			else if(strCmd=="SET_MANUAL_MODE") { // set default BlueBoat manual mode
//				process_set_manual_mode();
//			}
//			else if(strCmd=="ARM") { // set motors armed
//				process_arm();
//			}
//			else if(strCmd=="DISARM") { // set motors disarmed
//				process_disarm();
//			}
//			else if(strCmd=="MAP_SERVO_DEFAULT") { // set map servo default
//				process_map_servo_default();
//			}
//			else if(strCmd=="MAP_SERVO_RC") { // set map servo raw
//				process_map_servo_rc();
//			}
//			else {
////				printf("BlueBoatTCAthread::decode ERROR read - BLUEBOAT_TCA cmd is %s\n",strCmd.c_str());
////				fflush(stdout);
//				return ;
//			}
//		}
//	} // if(strType=="BLUEBOAT_TCA")
//	else  {
////		printf("BlueBoatTCAthread::process_command ERROR read - type is %s\n",strType.c_str());
////		fflush(stdout);
//		return ;
//	}
//
//}


//void BlueBoatLPRIthread::process_data(char *data,ssize_t nRecv) {
////	printf("BlueBoatTCAthread::process_data nRecv %d data %s\n",(int)nRecv,data);
//	NormalisedRpm azimuthData;
//	char dataType[64];
//	sscanf(data,"%s",dataType);
//	string dataStrType=dataType;
//	if(dataStrType=="NORM_RPM") {
//		mBlueBoatRpmMeas.mTimeStamp = mTimeStamp;
//		mBlueBoatRpmMeas.sread(data);
//		mBlueBoatRpmMeas.set_name("msr_"+mName);
//	}
////	else if((dataStrType=="AHRS")||(dataStrType=="GNSS_POS")||(dataStrType=="BATTERY")) {
////		// do nothing
////	}
//	else {
//		printf("mBlueBoatRpmMeas::process_data unexpected data type %s\n",dataStrType.c_str());
//		fflush(stdout);
//	}
//}


//void BlueBoatLPRIthread::execute_thread_semantics(void) {
////	char str[512];
////	mSwampActRef.sprint_cr_lf(str);
////	printf("%s",str);fflush(stdout);
//	LPRIthread::execute_thread_semantics();
//	//
//	// execute Thrust Control Allocation
//	//
////	mTCA->execute();
//
//}


void BlueBoatLPRIthread::build_commands(void) {
//	printf("BlueBoatLPRIthread::build_commands - mTCAstate.mEnable is %d\n",
//			(int)mTCAstate.mEnable);fflush(stdout);

	if(!mTCAstate.mEnable) {
//		printf("BlueBoatLPRIthread::build_commands - mTCAstate.mEnable is false\n");fflush(stdout);
		return;
	}
	//
	// append reference commands to mCmdSendBuffer
	//
	Packet packet;
	//
	// BlueBoat
	// left and right PWM
	//
	BlueBoatCmd mBlueBoatCmd;
	mBlueBoatCmd.leftRpm = mBlueBoatRpmRef.mLeftRpm;
	mBlueBoatCmd.rightRpm = mBlueBoatRpmRef.mRightRpm;
	packet.n=mBlueBoatCmd.code_set_manual_ref((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
//	printf("BlueBoatLPRIthread::build_commands: %s packet %s",mName.c_str(),(char*)packet.data);

}


void BlueBoatLPRIthread::build_data(void) {
	//
	// build generic TCA data and append output to mDataSendBuffer
	//
	LPRIthread::build_data();

	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	packet.n=mBlueBoatRpmRef.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("BlueBoatLPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mBlueBoatRpmMeas.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("BlueBoatLPRIthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
}


//void BlueBoatTCAthread::process_set_manual_mode(void) {
//	// set default manual mode
//	Packet packet;
//	BlueBoatCmd blueBoatCmd;
//	blueBoatCmd.mode=BLUE_BOAT_MANUAL;
//	packet.n=blueBoatCmd.code_set_mode((char*)packet.data);
//	mCmdSendBuffer[0].push(packet);
//}


//void BlueBoatTCAthread::process_arm(void) {
//	// set motors armed
//	Packet packet;
//	BlueBoatCmd blueBoatCmd;
//	blueBoatCmd.arm=BLUE_BOAT_ARM;
//	packet.n=blueBoatCmd.code_set_arm((char*)packet.data);
//	mCmdSendBuffer[0].push(packet);
//}


//void BlueBoatTCAthread::process_disarm(void) {
//	// set motors disarmed
//	Packet packet;
//	BlueBoatCmd blueBoatCmd;
//	blueBoatCmd.arm=BLUE_BOAT_DISARM;
//	packet.n=blueBoatCmd.code_set_arm((char*)packet.data);
//	mCmdSendBuffer[0].push(packet);
//}


//void BlueBoatTCAthread::process_map_servo_default(void) {
//	// set map servo default
//	Packet packet;
//	BlueBoatCmd blueBoatCmd;
//	packet.n=blueBoatCmd.code_map_servo_default((char*)packet.data);
//	mCmdSendBuffer[0].push(packet);
//}


//void BlueBoatTCAthread::process_map_servo_raw(void) {
//	// set map servo raw
//	Packet packet;
//	BlueBoatCmd blueBoatCmd;
//	packet.n=blueBoatCmd.code_map_servo_rc((char*)packet.data);
//	mCmdSendBuffer[0].push(packet);
//}


//void BlueBoatTCAthread::process_set_left_right_rpm(void) {
//	// set left & right rpm
//	// TCA mode is automatically set to manual
//	((BlueBoatTCA*)mTCA)->set_manual_default();
//	((BlueBoatTCA*)mTCA)->set_left_right_rpm(mBlueBoatTcaCmd.nL,mBlueBoatTcaCmd.nR);
//}


