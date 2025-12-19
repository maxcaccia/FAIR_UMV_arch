/*
 * BlueBoatThread.cpp
 *
 *  Created on: Mar23, 2025
 *      Author: mc
 */

//***** insert your code here ************************************

#include "BlueBoatThread.h"

BlueBoatThread::BlueBoatThread(void): DriverThread() {
//***** end insert your code *************************************
//	printf("BlueBoatThread::BlueBoatThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);

	//***** insert your code here ************************************
	//
	// instantiate device data
	//
	mHeartBeatFlag=false;
	mNormRpm = new NormalisedRpm();
	mAhrsData = new AhrsData();
	mGnssPosData = new GnssPositionData();
	mGnssVelData = new GnssVelocityData();
	mBatteryData = new BatteryData();

	mNormRpmRef = new NormalisedRpm();
	mNormRpmRef->set_name(mName);

	mNopCounter=0;

	//***** end insert your code *************************************

}

//***** insert your code here ************************************
BlueBoatThread::~BlueBoatThread() {
//***** end insert your code *************************************
	// TODO Auto-generated destructor stub
}

//***** insert your code here ************************************
void BlueBoatThread::make_thread()
{
	printf("BlueBoatThread::make_thread %s\n",mName.c_str());fflush(stdout);
	mThread = new thread(&BlueBoatThread::execute,this);
//	mThread = new thread(&execute,this);
	printf("BlueBoatThread::make_thread %s done\n",mName.c_str());fflush(stdout);

}
//***** end insert your code *************************************


//***** insert your code here ************************************
//
void BlueBoatThread::end(void)
{
	printf("BlueBoatThread::end %s\n",mName.c_str());
//***** end insert your code *************************************

	DriverThread::end();
	//***** insert your code here ************************************
	if(mNormRpm!=NULL) {
		delete mNormRpm;
		mNormRpm=NULL;
	}
	if(mAhrsData!=NULL) {
		delete mAhrsData;
		mAhrsData=NULL;
	}
	if(mGnssPosData!=NULL) {
		delete mGnssPosData;
		mGnssPosData=NULL;
	}
	if(mGnssVelData!=NULL) {
		delete mGnssVelData;
		mGnssVelData=NULL;
	}
	if(mBatteryData!=NULL) {
		delete mBatteryData;
		mBatteryData=NULL;
	}
	if(mNormRpmRef!=NULL) {
		delete mNormRpmRef;
		mNormRpmRef=NULL;
	}
	//***** end insert your code *************************************
}


//***** insert your code here ************************************
void BlueBoatThread::execute_thread_semantics(void) {
//***** end insert your code *************************************
//	printf("BlueBoatThread::execute_thread_semantics() %s\n",mName.c_str());
	Packet packet;
	string strPacket;

	//
	// check HeartBeat
	//
	if(mHeartBeatFlag) {
		mavlink_heartbeat_t hbSend;
		mavlink_message_t msgSend;
//		uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
		hbSend.type = 0;
		hbSend.custom_mode = 0;
		hbSend.base_mode = 0;
		hbSend.mavlink_version = 0;
		hbSend.system_status = 0;
		hbSend.autopilot = 0;
		mavlink_msg_heartbeat_encode(0xff, 0xe6, &msgSend, &hbSend);
		//int len = mavlink_msg_to_send_buffer((uint8_t*)buffer, &msgSend);
		packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msgSend);

		//packet.n=sprintf((char*)packet.data,"%d %d %" PRId32  "\r\n",COMMAND_MOTOR_SET_REFERENCE_OPCODE,AZIMUTHAL_MOTOR,azimuthMotorSetRefTick);
		mCmdSendBuffer.push(packet);
//		printf("BlueBoatThread::execute_thread_semantics send heart-beat\n");fflush(stdout);
		mHeartBeatFlag=false;
//		sock->sendto(robot_addr, (char*)buffer, len);
	}

	//***** insert your code here ************************************

	//
	// get data from data parser
	//
	((BlueBoatDataParser*)mDataParser)->get_heartbeat(mHeartBeatFlag);
//	((BlueBoatDataParser*)mDataParser)->get_tlm_data((NormalisedRpm&)*mNormRpm);
	((BlueBoatDataParser*)mDataParser)->get_data((NormalisedRpm&)*mNormRpm);
	((BlueBoatDataParser*)mDataParser)->get_data((AhrsData&)*mAhrsData);
	((BlueBoatDataParser*)mDataParser)->get_data((GnssPositionData&)*mGnssPosData);
	((BlueBoatDataParser*)mDataParser)->get_data((GnssVelocityData&)*mGnssVelData);
	((BlueBoatDataParser*)mDataParser)->get_data((BatteryData&)*mBatteryData);
	set_data_names();
	//***** end insert your code *************************************

//	printf("BlueBoatThread::execute_thread_semantics mAhrsData->mUpdated %d\n",
//			(int)mAhrsData->mUpdated); fflush(stdout);

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

void BlueBoatThread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- BlueBoatThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	Packet packet;
	string strPacket;

//	//
//	// check HeartBeat
//	//
//	if(mHeartBeatFlag) {
//		mavlink_heartbeat_t hbSend;
//		mavlink_message_t msgSend;
////		uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
//		hbSend.type = 0;
//		hbSend.custom_mode = 0;
//		hbSend.base_mode = 0;
//		hbSend.mavlink_version = 0;
//		hbSend.system_status = 0;
//		hbSend.autopilot = 0;
//		mavlink_msg_heartbeat_encode(0xff, 0xe6, &msgSend, &hbSend);
//		//int len = mavlink_msg_to_send_buffer((uint8_t*)buffer, &msgSend);
//		packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msgSend);
//
//		//packet.n=sprintf((char*)packet.data,"%d %d %" PRId32  "\r\n",COMMAND_MOTOR_SET_REFERENCE_OPCODE,AZIMUTHAL_MOTOR,azimuthMotorSetRefTick);
//		mCmdSendBuffer.push(packet);
//		printf("BlueBoatThread::process_command send heart-beat\n");fflush(stdout);
//
////		sock->sendto(robot_addr, (char*)buffer, len);
//	}

	char chrType[64], chrName[64];
	string strType,strName,strCmd;
	sscanf(cmd,"%s %s",chrType,chrName);
	strType=chrType;
	strName=chrName;

	if(strType=="BLUE_BOAT") {
		if(mBlueBoatCmd.decode(cmd, strCmd)) {
			//
			// build command for the device
			//
			if(strCmd=="SET_MODE") {
//				uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
				mavlink_command_long_t rc;
				rc.target_system = 1;
				rc.target_component = 1;
				rc.command = MAV_CMD_DO_SET_MODE;
				rc.confirmation = 0;
				*(uint32_t*)(&(rc.param1)) = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED; // 0x3f800000; MAV_MODE_FLAG_CUSTOM_MODE_ENABLED

				if (mBlueBoatCmd.mode == BLUE_BOAT_MANUAL) *(uint32_t*)(&(rc.param2)) = 0x00000000; // 0
				else if (mBlueBoatCmd.mode == BLUE_BOAT_HOLD) *(uint32_t*)(&(rc.param2)) = 0x40800000; // 4
				else if (mBlueBoatCmd.mode == BLUE_BOAT_GUIDED) *(uint32_t*)(&(rc.param2)) = 0x41700000; // 15

				*(uint32_t*)(&(rc.param3)) = 0x00000000;
				*(uint32_t*)(&(rc.param4)) = 0x00000000;
				*(uint32_t*)(&(rc.param5)) = 0x00000000;
				*(uint32_t*)(&(rc.param6)) = 0x00000000;
				*(uint32_t*)(&(rc.param7)) = 0x00000000;

				mavlink_message_t msg;
				mavlink_msg_command_long_encode(0xff, 0xe6, &msg, &rc);
				//int len=mavlink_msg_to_send_buffer((uint8_t*)buffer, &msg);
				//sock->sendto(robot_addr, (char*)buffer, len);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);
//printf("BlueBoatThread::process_command %s %s %" PRIu32 "\n",strType.c_str(),mBlueBoatCmd.mode.c_str(),msg.msgid);
//printf("BlueBoatThread::process_command %s %s %" PRIu16 "\n",strType.c_str(),mBlueBoatCmd.mode.c_str(),rc.command);
//printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu32 " %" PRIu32 " %" PRIu32 "\n",rc.param1,rc.param2,rc.param3);fflush(stdout);
			}
			else if(strCmd=="SET_ARM") {
//				uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
				mavlink_command_long_t rc;

				rc.target_system = 1;
				rc.target_component = 1;
				rc.command = MAV_CMD_COMPONENT_ARM_DISARM;//0x0190;//0x019a;//0x00b0; // MAV_CMD_DO_SET_MODE;
				rc.confirmation = 0;
//				*(uint32_t*)(&(rc.param1)) = 0x3f800000;// MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;

				if (mBlueBoatCmd.arm==BLUE_BOAT_ARM)
					{
//					printf("arm\n");
					rc.param1 = 1.0;
//					*(uint32_t*)(&(rc.param1)) = 1000;
					}
				else if (mBlueBoatCmd.arm == BLUE_BOAT_DISARM) {
//					printf("disarm\n");
					rc.param1 = 0.0;
				}

				//rc.param2=21196;
				// force arm/disarm
				*(uint32_t*)(&(rc.param2)) = 21196;
				*(uint32_t*)(&(rc.param3)) = 0x00000000;
				*(uint32_t*)(&(rc.param4)) = 0x00000000;
				*(uint32_t*)(&(rc.param5)) = 0x00000000;
				*(uint32_t*)(&(rc.param6)) = 0x00000000;
				*(uint32_t*)(&(rc.param7)) = 0x00000000;

				mavlink_message_t msg;
				mavlink_msg_command_long_encode(0xff, 0xe6, &msg, &rc);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);
//				printf("BlueBoatThread::process_command %s %s %" PRIu32 "\n",strType.c_str(),mBlueBoatCmd.arm.c_str(),msg.msgid);

//				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu32 " %" PRIu32 " %" PRIu32 "\n",rc.param1,rc.param2,rc.param3);fflush(stdout);

			}
//			else if(strCmd=="SET_THRUST_TORQUE") {
//				mForceTorqueRef->X = mBlueBoatCmd.thrust;
//				mForceTorqueRef->N = mBlueBoatCmd.torque;
//				mForceTorqueRef->mUpdated = true;
//
////				uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
//
//				mavlink_rc_channels_override_t rc;
//
//				uint16_t fu = (uint16_t)(PWM_ZERO + 4.0 * mBlueBoatCmd.thrust);
//				uint16_t tr = (uint16_t)(PWM_ZERO + 4.0 * mBlueBoatCmd.torque);
//
//				rc.target_system = 1;
//				rc.target_component = 0;  // 0
//				rc.chan1_raw = tr;
//				rc.chan2_raw = PWM_ZERO;
//				rc.chan3_raw = fu;
//				rc.chan4_raw = PWM_ZERO;
//
//				rc.chan5_raw = PWM_ZERO;    rc.chan6_raw = PWM_ZERO;
//				rc.chan7_raw = PWM_ZERO;    rc.chan8_raw = PWM_ZERO;
//				rc.chan9_raw = 0;       rc.chan10_raw = 0;      rc.chan11_raw = 0;      rc.chan12_raw = 0;
//				rc.chan13_raw = 0;      rc.chan14_raw = 0;      rc.chan15_raw = 0;      rc.chan16_raw = 0;
//				rc.chan17_raw = 0;      rc.chan18_raw = 0;
//
//				mavlink_message_t msg;
//				mavlink_msg_rc_channels_override_encode(0xff, 0xe6, &msg, &rc);
//
//				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
//				mCmdSendBuffer.push(packet);
//			}
			else if(strCmd=="MAP_SERVO_DEFAULT") {
				mavlink_message_t msg;
				// SERVO1_FUNCTION --> Throttle Right
				mavlink_msg_param_set_pack(1, 200, &msg, 1, 1,
						"SERVO1_FUNCTION", THROTTLE_RIGHT, MAV_PARAM_TYPE_INT32);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);
				// SERVO3_FUNCTION --> Throttle Left
				mavlink_msg_param_set_pack(1, 200, &msg, 1, 1,
						"SERVO3_FUNCTION", THROTTLE_LEFT, MAV_PARAM_TYPE_INT32);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);
//				// SERVO3_FUNCTION --> Reversed
//				mavlink_msg_param_set_pack(1, 200, &msg, 1, 1,
//						"SERVO3_REVERSED", 1, MAV_PARAM_TYPE_INT32);
//				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
//				mCmdSendBuffer.push(packet);

			}
			else if(strCmd=="MAP_SERVO_RAW") {

				mavlink_message_t msg;
				// SERVO3_FUNCTION --> disabled
				mavlink_msg_param_set_pack(1, 200, &msg, 1, 1,
						"SERVO3_FUNCTION", 0, MAV_PARAM_TYPE_INT32);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);
				// SERVO1_FUNCTION --> disabled
				mavlink_msg_param_set_pack(1, 200, &msg, 1, 1,
						"SERVO1_FUNCTION", 0, MAV_PARAM_TYPE_INT32);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);

				mCmdSendBuffer.push(packet);
//				// SERVO3_FUNCTION --> Normal
//				mavlink_msg_param_set_pack(1, 200, &msg, 1, 1,
//						"SERVO3_REVERSED", 0, MAV_PARAM_TYPE_INT32);
//				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
//				mCmdSendBuffer.push(packet);

			}
			else if(strCmd=="SET_MANUAL_REF") {
				//
				// set servo
				//
				mavlink_command_long_t rc;

				rc.target_system = 1;
				rc.target_component = 1;
				rc.command = MAV_CMD_DO_SET_SERVO;
				rc.confirmation = 0;

				// left thruster is reversed
				float deltaLeft = - mBlueBoatCmd.leftRpm/100.;
				float deltaRight = mBlueBoatCmd.rightRpm/100.;
				float left, right;
				if(deltaLeft < 0.)
					left = PWM_ZERO + (PWM_ZERO-PWM_MIN)*deltaLeft;
				else
					left = PWM_ZERO + (PWM_MAX-PWM_ZERO)*deltaLeft;
				if(deltaRight < 0.)
					right = PWM_ZERO + (PWM_ZERO-PWM_MIN)*deltaRight;
				else
					right = PWM_ZERO + (PWM_MAX-PWM_ZERO)*deltaRight;
					
				//float left = PWM_ZERO - PWM_DELTA_100 * mBlueBoatCmd.leftRpm;
				//float right = PWM_ZERO + PWM_DELTA_100 * mBlueBoatCmd.rightRpm;

				mNormRpmRef->mLeftRpm = mBlueBoatCmd.leftRpm;
				mNormRpmRef->mRightRpm = mBlueBoatCmd.rightRpm;

				mavlink_message_t msg;

				rc.param1=1.;
				rc.param2=right;
				mavlink_msg_command_long_encode(0xff, 0xe6, &msg, &rc);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);

				rc.param1=3.;
				rc.param2=left;
				mavlink_msg_command_long_encode(0xff, 0xe6, &msg, &rc);
				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);


//
// channels overwrite
//
//				mavlink_rc_channels_override_t rc;
//
//				uint16_t left = (uint16_t)(PWM_ZERO + 5.0 * mBlueBoatCmd.leftRpm);
//				uint16_t right = (uint16_t)(PWM_ZERO + 5.0 * mBlueBoatCmd.rightRpm);
//
//				rc.target_system = 1;
//				rc.target_component = 0;
//				rc.chan1_raw = right;
//				rc.chan2_raw = PWM_ZERO;
//				rc.chan3_raw = left;
//				rc.chan4_raw = PWM_ZERO;
//
//				rc.chan5_raw = PWM_ZERO;    rc.chan6_raw = PWM_ZERO;    rc.chan7_raw = PWM_ZERO;    rc.chan8_raw = PWM_ZERO;
//				rc.chan9_raw = 0;       rc.chan10_raw = 0;      rc.chan11_raw = 0;      rc.chan12_raw = 0;
//				rc.chan13_raw = 0;      rc.chan14_raw = 0;      rc.chan15_raw = 0;      rc.chan16_raw = 0;
//				rc.chan17_raw = 0;      rc.chan18_raw = 0;
//
//				mavlink_message_t msg;
//				mavlink_msg_rc_channels_override_encode(0xff, 0xe6, &msg, &rc);
//
//				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
//				mCmdSendBuffer.push(packet);
			}


			else if(strCmd=="SET_WAYPOINT") {
				mavlink_message_t msg;

				int32_t lat_int = (int32_t)(mBlueBoatCmd.lat *1E7);
				int32_t lon_int = (int32_t)(mBlueBoatCmd.lon *1E7);

				mavlink_msg_set_position_target_global_int_pack(
						1,
						1,
						&msg,
						0, // time boot ms
						1,
						1,
						MAV_FRAME_GLOBAL_RELATIVE_ALT_INT,
						0b0000111111111000, // type mask
						lat_int,
						lon_int,
						0,
						0, 0, 0, // velocity
						0, 0, 0, // acceleration
						0, 0     // yaw, yaw rate
						);

				packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
				mCmdSendBuffer.push(packet);
			}


			else  {
				printf("BlueBoatThread::decode ERROR read - BLUE_BOAT cmd is %s\n",strCmd.c_str());
				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="BLUE_BOAT")
	else  {
		printf("BlueBoatThread::decode ERROR read - type is %ss\n",strType.c_str());
		fflush(stdout);
		return ;
	}

}


void BlueBoatThread::build_data(void) {
	//
	// append output to mDataSendBuffer
	//
	Packet packet;

	if(mNormRpm->mUpdated) {
		packet.n=mNormRpm->sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mNormRpm->mUpdated = false;
		//printf("BlueBoatThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);fflush(stdout);
	}

//	printf("BlueBoatThread::build_data %d\n",(int)mAhrsData->mUpdated);fflush(stdout);
	if(mAhrsData->mUpdated) {
		packet.n=mAhrsData->sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mAhrsData->mUpdated = false;
//		printf("BlueBoatThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);fflush(stdout);
	}

	if(mGnssPosData->mUpdated) {
		packet.n=mGnssPosData->sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mGnssPosData->mUpdated = false;
	//printf("BlueBoatThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);fflush(stdout);
	}

	if(mGnssVelData->mUpdated) {
		packet.n=mGnssVelData->sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mGnssVelData->mUpdated = false;
	//printf("BlueBoatThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);fflush(stdout);
	}

	if(mBatteryData->mUpdated) {
		packet.n=mBatteryData->sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mBatteryData->mUpdated = false;
	//printf("BlueBoatThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);fflush(stdout);
	}

}


void BlueBoatThread::set_data_names(void) {
	mNormRpm->set_name(mName);
	mAhrsData->set_name(mName);
	mGnssPosData->set_name(mName);
	mGnssVelData->set_name(mName);
	mBatteryData->set_name(mName);
}
