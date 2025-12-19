/*
 * MinionUDPthread.cpp
 *
 *  Created on: Feb 5, 2024
 *      Author: mc
 */

//***** insert your code here ************************************

#include "MinionUDPthread.h"

MinionUDPthread::MinionUDPthread(void): DriverThread() {
//***** end insert your code *************************************
	printf("MinionUDPthread::MinionUDPthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);

	//***** insert your code here ************************************
	//
	// instantiate device data
	//
	mPositionMotorData = new PositionMotorData();
	mSpeedMotorData = new SpeedMotorData();
	mAhrsData = new AhrsData();
	mGnssPosData = new GnssPositionData();
	mBatteryData = new BatteryData();

	mAzimuthRefUnwrap=0;
	mNopCounter=0;

	//***** end insert your code *************************************

}

//***** insert your code here ************************************
MinionUDPthread::~MinionUDPthread() {
//***** end insert your code *************************************
	// TODO Auto-generated destructor stub
}

//***** insert your code here ************************************
void MinionUDPthread::make_thread()
{
	printf("MinionUDPthread::make_thread %s\n",mName.c_str());fflush(stdout);
	mThread = new thread(&MinionUDPthread::execute,this);
//	mThread = new thread(&execute,this);
	printf("MinionUDPthread::make_thread %s done\n",mName.c_str());fflush(stdout);

}
//***** end insert your code *************************************


//***** insert your code here ************************************
//
void MinionUDPthread::end(void)
{
	printf("MinionUDPthread::end %s\n",mName.c_str());
//***** end insert your code *************************************

	DriverThread::end();
	//***** insert your code here ************************************
	if(mPositionMotorData!=NULL) {
		delete mPositionMotorData;
		mPositionMotorData=NULL;
	}
	if(mSpeedMotorData!=NULL) {
		delete mSpeedMotorData;
		mSpeedMotorData=NULL;
	}
	if(mAhrsData!=NULL) {
		delete mAhrsData;
		mAhrsData=NULL;
	}
	if(mGnssPosData!=NULL) {
		delete mGnssPosData;
		mGnssPosData=NULL;
	}
	if(mBatteryData!=NULL) {
		delete mBatteryData;
		mBatteryData=NULL;
	}
	//***** end insert your code *************************************
}


//***** insert your code here ************************************
void MinionUDPthread::execute_thread_semantics(void) {
//***** end insert your code *************************************
//	printf("MinionUDPthread::execute_thread_semantics() %s\n",mName.c_str());

	//***** insert your code here ************************************

	//
	// get data from data parser
	//
	((MinionDataParser*)mDataParser)->get_tlm_data((PositionMotorData&)*mPositionMotorData);
	((MinionDataParser*)mDataParser)->get_tlm_data((SpeedMotorData&)*mSpeedMotorData);
	((MinionDataParser*)mDataParser)->get_data((AhrsData&)*mAhrsData);
	((MinionDataParser*)mDataParser)->get_data((GnssPositionData&)*mGnssPosData);
	((MinionDataParser*)mDataParser)->get_data((BatteryData&)*mBatteryData);
	set_data_names();
	//***** end insert your code *************************************

	//***** insert your code here ************************************
	//
	//
	// execute semantics
	//
	mNopCounter++;

	// nothing to do, in this case

	//***** end insert your code *************************************

	//***** insert your code here ************************************

}

void MinionUDPthread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- MinionUDPthread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	Packet packet;
	string strPacket;

	char chrType[64], chrName[64];
	string strType,strName,strCmd;
	sscanf(cmd,"%s %s",chrType,chrName);
	strType=chrType;
	strName=chrName;

//	if(!(strName==mName)) {
//		printf("MinionUDPthread::decode ERROR read - name is %s instead of %s\n",strName.c_str(),mName.c_str());
//		fflush(stdout);
//		return ;
//	}

	if(strType=="POS_MOT") {
		if(mPositionMotorCmd.decode(cmd, strCmd)) {
			//
			// build command for the device
			//
			if(strCmd=="SET_ANGLE") {
				double alphaRef;
				int32_t azimuthMotorSetRefTick;
				alphaRef=mPositionMotorCmd.angle;
				unwrap(alphaRef,mAzimuthRefUnwrap);
				azimuthMotorSetRefTick=round(mAzimuthRefUnwrap*TICKS_PER_ROUND/360.);
				packet.n=sprintf((char*)packet.data,"%d %d %" PRId32  "\r\n",COMMAND_MOTOR_SET_REFERENCE_OPCODE,AZIMUTHAL_MOTOR,azimuthMotorSetRefTick);
				mCmdSendBuffer.push(packet);
			}
			else if(strCmd=="GO_HOME") {
				packet.n=sprintf((char*)packet.data,"%d\r\n",COMMAND_AZIMUTHAL_MOTOR_GO_HOME_OPCODE);
				mCmdSendBuffer.push(packet);
			}
			else if(strCmd=="SET_HOME") {
				packet.n=sprintf((char*)packet.data,"%d\r\n",COMMAND_AZIMUTHAL_MOTOR_SET_HOME_OPCODE);
				mCmdSendBuffer.push(packet);
				// set reference to zero
				mPositionMotorCmd.angle=0.;
				mAzimuthRefUnwrap=0.;
			}
			else if(strCmd=="POWER") {
				packet.n=sprintf((char*)packet.data,"%d %d %" PRIu8  "\r\n",COMMAND_MOTOR_POWER_OPCODE,AZIMUTHAL_MOTOR,mPositionMotorCmd.power);
				mCmdSendBuffer.push(packet);
			}
			else if(strCmd=="ENABLE") {
				packet.n=sprintf((char*)packet.data,"%d %d %" PRIu8  "\r\n",COMMAND_MOTOR_ENABLE_OPCODE,AZIMUTHAL_MOTOR,mPositionMotorCmd.enable);
				mCmdSendBuffer.push(packet);
				if(mPositionMotorCmd.enable==1) {
					mPositionMotorCmd.maxSpeed=MAX_POS_MOT_SPEED;
					packet.n=sprintf((char*)packet.data,"%d %" PRId32  "\r\n",COMMAND_AZIMUTHAL_MOTOR_SET_MAX_SPEED_OPCODE,(int32_t)mPositionMotorCmd.maxSpeed);
					mCmdSendBuffer.push(packet);
				}
			}
			else if(strCmd=="SET_MAX_SPEED") {
				packet.n=sprintf((char*)packet.data,"%d %" PRId32  "\r\n",COMMAND_AZIMUTHAL_MOTOR_SET_MAX_SPEED_OPCODE,(int32_t)mPositionMotorCmd.maxSpeed);
				mCmdSendBuffer.push(packet);
			}
			else  {
				printf("MinionUDPthread::decode ERROR read - POS_MOT cmd is %s\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="POS_MOT")
	else if(strType=="SPEED_MOT") {
		if(mSpeedMotorCmd.decode(cmd, strCmd)) {
			if(mSpeedMotorCmd.decode(cmd, strCmd)) {
				//
				// build command for the device
				//
				if(strCmd=="SET_SPEED") {
					packet.n=sprintf((char*)packet.data,"%d %d %f\r\n",COMMAND_MOTOR_SET_REFERENCE_OPCODE,THRUST_MOTOR,100.*1000.*mSpeedMotorCmd.speed/THRUST_RPM_MAX);
					mCmdSendBuffer.push(packet);
				}
				else if(strCmd=="POWER") {
					packet.n=sprintf((char*)packet.data,"%d %d %" PRIu8  "\r\n",COMMAND_MOTOR_POWER_OPCODE,THRUST_MOTOR,mSpeedMotorCmd.power);
					mCmdSendBuffer.push(packet);
				}
				else if(strCmd=="ENABLE") {
					packet.n=sprintf((char*)packet.data,"%d %d %" PRIu8  "\r\n",COMMAND_MOTOR_ENABLE_OPCODE,THRUST_MOTOR,mSpeedMotorCmd.enable);
					mCmdSendBuffer.push(packet);
				}
				else  {
					printf("MinionUDPthread::decode ERROR read - SPEED_MOT cmd is %ss\n",strCmd.c_str());
					fflush(stdout);
					return ;
				}
			}

		}
	} // if(strType=="SPEED_MOT")
	else if(strType=="GENERIC") {
		if(mGenericCmd.decode(cmd, strCmd)) {
			if(strCmd=="NOP") {
				mNopCounter=0;
				packet.n=sprintf((char*)packet.data,"%d\r\n",COMMAND_NOP_OPCODE);
				mCmdSendBuffer.push(packet);
			}
			else  {
				printf("MinionUDPthread::decode ERROR read - SPEED_MOT cmd is %ss\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="GENERIC")
	else  {
		printf("MinionUDPthread::decode ERROR read - type is %ss\n",strType.c_str());
		fflush(stdout);
		return ;
	}

}


void MinionUDPthread::build_data(void) {
	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	packet.n=mPositionMotorData->sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("MinionUDPthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mSpeedMotorData->sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("MinionUDPthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mAhrsData->sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("MinionUDPthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mGnssPosData->sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("MinionUDPthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mBatteryData->sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
//	printf("MinionUDPthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
}


void MinionUDPthread::set_data_names(void) {
	mPositionMotorData->set_name(mName);
	mSpeedMotorData->set_name(mName);
	mAhrsData->set_name(mName);
	mGnssPosData->set_name(mName);
	mBatteryData->set_name(mName);
}
