/*
 * SwampTCAthread.cpp
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#include "SwampTCAthread.h"
#include "SwampTCA.h"

SwampTCAthread::SwampTCAthread(void): TCAthread() {
//	mEnableRpmAzimuth=false;
	mTCA = new SwampTCA();
//printf("SwampTCAthread::SwampTCAthread mTCA %" PRIu64"\n",(uint64_t)mTCA);fflush(stdout);
//	mTCA = new SwampThrustControlAllocation(mName,&mSwampActRef);
//	((SwampThrustControlAllocation*)mTCA)->set_input_output(&mForceTorqueRef, &mSwampActRef);
//	printf("SwampTCAthread::SwampTCAthread end %s %d %d\n",mName.c_str(),mPolicy,mPriority);fflush(stdout);
}

SwampTCAthread::~SwampTCAthread() {
	// TODO Auto-generated destructor stub
	if(mTCA!=NULL) {
		delete mTCA;
		mTCA=NULL;
	}
}

void SwampTCAthread::make_thread()
{
//    printf("SwampTCAthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&SwampTCAthread::execute,this);
}

bool SwampTCAthread::init_configuration(void) {
//printf("SwampTCAthread::init_configuration %s\n",mName.c_str());fflush(stdout);
	//
	mTCA->set_name(mName);

	TCAthread::init_configuration();

	mSwampActRef.set_name("ref_"+mName);
	mSwampActMeas.set_name("msr_"+mName);

	((SwampTCA*)mTCA)->set_input_output(&mSwampActRef,&mSwampActMeas);

	mTCA->reset();

	return true;
}


void SwampTCAthread::process_command(char *cmd,ssize_t n) {
	TCAthread::process_command(cmd,n);

//	printf("******************** %s ---- SwampTCAthread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="SWAMP_TCA") {
		if(mSwampTcaCmd.decode(cmd, strCmd)) {
			if(strCmd=="SET_RPM_AZIMUTH") {
				mSwampActRef.mTimeStamp = mTimeStamp;
				process_set_rpm_azimuth();
			}
			else if(strCmd=="SET_POWER") {
				process_set_power();
			}
			else if(strCmd=="SET_ENABLE") {
				process_set_enable();
			}
			else if(strCmd=="HOMING") {
				process_homing();
			}
			else if(strCmd=="SET_AZIMUTH_OFFSET") {
				process_set_azimuth_offset();
			}
			else if(strCmd=="SET_AZIMUTH_HOME") {
				process_set_azimuth_home();
			}
			else {
//				printf("SwampTCAthread::decode ERROR read - SWAMP_TCA cmd is %s\n",strCmd.c_str());
//				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="SWAMP_TCA")
	else  {
//		printf("SwampTCAthread::process_command ERROR read - type is %s\n",strType.c_str());
//		fflush(stdout);
		return ;
	}

}


void SwampTCAthread::process_set_power(void) {
	//
	// build command
	//
	Packet packet;
	if(mSwampTcaCmd.motorType=="position") {
		mAzimuthCmd.power=mSwampTcaCmd.power;
		packet.n=mAzimuthCmd.code_power((char*)packet.data);
	}
	else if(mSwampTcaCmd.motorType=="speed") {
		mPumpCmd.power=mSwampTcaCmd.power;
		packet.n=mPumpCmd.code_power((char*)packet.data);
	}
	else
		return;
	//
	// send command to minion(s)
	//
	if(mSwampTcaCmd.motorId=="all") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
		mCmdSendBuffer[SWAMP_FL].push(packet);
		mCmdSendBuffer[SWAMP_RR].push(packet);
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RL") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FL") {
		mCmdSendBuffer[SWAMP_FL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RR") {
		mCmdSendBuffer[SWAMP_RR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FR") {
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else
		return;
}


void SwampTCAthread::process_set_enable(void) {
	//
	// build command
	//
	Packet packet;
	if(mSwampTcaCmd.motorType=="position") {
		mAzimuthCmd.enable=mSwampTcaCmd.enable;
		packet.n=mAzimuthCmd.code_enable((char*)packet.data);
	}
	else if(mSwampTcaCmd.motorType=="speed") {
		mPumpCmd.enable=mSwampTcaCmd.enable;
		packet.n=mPumpCmd.code_enable((char*)packet.data);
	}
	else
		return;
	//
	// send command to minion(s)
	//
	if(mSwampTcaCmd.motorId=="all") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
		mCmdSendBuffer[SWAMP_FL].push(packet);
		mCmdSendBuffer[SWAMP_RR].push(packet);
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RL") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FL") {
		mCmdSendBuffer[SWAMP_FL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RR") {
		mCmdSendBuffer[SWAMP_RR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FR") {
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else
		return;
}

void SwampTCAthread::process_homing(void) {
	//
	// build command
	//
	Packet packet;
	packet.n=mAzimuthCmd.code_go_home((char*)packet.data);
	//
	// send command to minion(s)
	//
	if(mSwampTcaCmd.motorId=="all") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
		mCmdSendBuffer[SWAMP_FL].push(packet);
		mCmdSendBuffer[SWAMP_RR].push(packet);
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RL") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FL") {
		mCmdSendBuffer[SWAMP_FL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RR") {
		mCmdSendBuffer[SWAMP_RR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FR") {
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else
		return;
}


void SwampTCAthread::process_set_azimuth_offset(void) {
	Packet packet;
	//
	// build & send command
	//
	for(int i=0;i<SWAMP_N;i++) {
		mAzimuthCmd.angle=mSwampTcaCmd.alphaOffset[i];
		packet.n=mAzimuthCmd.code_set_angle((char*)packet.data);
		mCmdSendBuffer[i].push(packet);
	}
}


void SwampTCAthread::process_set_azimuth_home(void) {
	//
	// build command
	//
	Packet packet;
	packet.n=mAzimuthCmd.code_set_home((char*)packet.data);
	//
	// send command to minion(s)
	//
	if(mSwampTcaCmd.motorId=="all") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
		mCmdSendBuffer[SWAMP_FL].push(packet);
		mCmdSendBuffer[SWAMP_RR].push(packet);
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RL") {
		mCmdSendBuffer[SWAMP_RL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FL") {
		mCmdSendBuffer[SWAMP_FL].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="RR") {
		mCmdSendBuffer[SWAMP_RR].push(packet);
	}
	else if(mSwampTcaCmd.motorId=="FR") {
		mCmdSendBuffer[SWAMP_FR].push(packet);
	}
	else
		return;
}


void SwampTCAthread::process_data(char *data,ssize_t nRecv) {
//	printf("SwampTCAthread::process_data nRecv %d data %s\n",(int)nRecv,data);
	PositionMotorData azimuthData;
	SpeedMotorData pumpData;
	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;
	if(dataStrType=="POS_MOT") {
		mSwampActMeas.mTimeStamp = mTimeStamp;
		azimuthData.sread(data);
		if(azimuthData.mName=="minionRL")
			mSwampActMeas.alphaRL=azimuthData.positionDeg;
		else if(azimuthData.mName=="minionFL")
			mSwampActMeas.alphaFL=azimuthData.positionDeg;
		else if(azimuthData.mName=="minionRR")
			mSwampActMeas.alphaRR=azimuthData.positionDeg;
		else if(azimuthData.mName=="minionFR")
			mSwampActMeas.alphaFR=azimuthData.positionDeg;
		else {
			printf("SwampTCAthread::process_data unexpected data name %s\n",azimuthData.mName.c_str());
			fflush(stdout);
		}
	}
	else if(dataStrType=="VEL_MOT") {
		mSwampActMeas.mTimeStamp = mTimeStamp;
		pumpData.sread(data);
		if(pumpData.mName=="minionRL")
			mSwampActMeas.nRL=pumpData.speed;
		else if(pumpData.mName=="minionFL")
			mSwampActMeas.alphaFL=pumpData.speed;
		else if(pumpData.mName=="minionRR")
			mSwampActMeas.alphaRR=pumpData.speed;
		else if(pumpData.mName=="minionFR")
			mSwampActMeas.alphaFR=pumpData.speed;
		else {
			printf("SwampTCAthread::process_data unexpected data name %s\n",pumpData.mName.c_str());
			fflush(stdout);
		}
	}
	else if((dataStrType=="AHRS")||(dataStrType=="GNSS_POS")||(dataStrType=="BATTERY")) {
		// do nothing
	}
	else {
		printf("SwampTCAthread::process_data unexpected data type %s\n",dataStrType.c_str());
		fflush(stdout);
	}
}


void SwampTCAthread::execute_thread_semantics(void) {
//	char str[512];
//	mSwampActRef.sprint_cr_lf(str);
//	printf("%s",str);fflush(stdout);
	TCAthread::execute_thread_semantics();
	//
	// execute Thrust Control Allocation
	//
	mTCA->execute();

}


void SwampTCAthread::build_commands(void) {
	if(!mTCAstate.mEnable) {
//		printf("SwampTCAthread::build_commands - mEnableRpmAzimuth is false\n");fflush(stdout);
		return;
	}
	//
	// append reference commands to mCmdSendBuffer
	//
	Packet packet;
	//
	// RL minion
	//
	// azimuth
	mAzimuthCmd.angle=mSwampActRef.alphaRL;
	packet.n=mAzimuthCmd.code_set_angle((char*)packet.data);
	mCmdSendBuffer[SWAMP_RL].push(packet);
	// pump
	mPumpCmd.speed=mSwampActRef.nRL;
	packet.n=mPumpCmd.code_set_speed((char*)packet.data);
	mCmdSendBuffer[SWAMP_RL].push(packet);
	//
	// FL minion
	//
	// azimuth
	mAzimuthCmd.angle=mSwampActRef.alphaFL;
	packet.n=mAzimuthCmd.code_set_angle((char*)packet.data);
	mCmdSendBuffer[SWAMP_FL].push(packet);
	// pump
	mPumpCmd.speed=mSwampActRef.nFL;
	packet.n=mPumpCmd.code_set_speed((char*)packet.data);
	mCmdSendBuffer[SWAMP_FL].push(packet);
	//
	// RR minion
	//
	// azimuth
	mAzimuthCmd.angle=mSwampActRef.alphaRR;
	packet.n=mAzimuthCmd.code_set_angle((char*)packet.data);
	mCmdSendBuffer[SWAMP_RR].push(packet);
	// pump
	mPumpCmd.speed=mSwampActRef.nRR;
	packet.n=mPumpCmd.code_set_speed((char*)packet.data);
	mCmdSendBuffer[SWAMP_RR].push(packet);
	//
	// FR minion
	//
	// azimuth
	mAzimuthCmd.angle=mSwampActRef.alphaFR;
	packet.n=mAzimuthCmd.code_set_angle((char*)packet.data);
	mCmdSendBuffer[SWAMP_FR].push(packet);
	// pump
	mPumpCmd.speed=mSwampActRef.nFR;
	packet.n=mPumpCmd.code_set_speed((char*)packet.data);
	mCmdSendBuffer[SWAMP_FR].push(packet);
}


void SwampTCAthread::build_data(void) {
	//
	// build generic TCA data and append output to mDataSendBuffer
	//
	TCAthread::build_data();

	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	packet.n=mSwampActRef.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
	printf("SwampTCAthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mSwampActMeas.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
	printf("SwampTCAthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
}

