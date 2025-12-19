/*
 * BlueBoatHwSimThread.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: mc
 */

#include "BlueBoatHwSimThread.h"


BlueBoatHwSimThread::BlueBoatHwSimThread(void): CntrlSimThread() {
//	printf("BlueBoatHwSimThread::BlueBoatHwSimThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	dt=0.;
	mId=0;
	mSamplingTime_ms=100;
	mMode="manual";
	mArm="disarmed";
	mServo1Function = THROTTLE_RIGHT;
	mServo3Function = THROTTLE_LEFT;

	mBatteryVoltage=BATTERY_VOLTAGE_FULL;
	mPose.reset();

	mGnssSim = new GnssSim();
	mAhrsSim = new AhrsSim();

	mHeartBeatFlag=false;

	mLeftRpm=mRightRpm=1500.;
}

BlueBoatHwSimThread::~BlueBoatHwSimThread() {
	// TODO Auto-generated destructor stub
	if(mGnssSim!=NULL)
		delete mGnssSim;
	if(mAhrsSim!=NULL)
		delete mAhrsSim;
}

void BlueBoatHwSimThread::make_thread()
{
//    printf("BlueBoatHwSimThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&BlueBoatHwSimThread::execute,this);
}

bool BlueBoatHwSimThread::init_configuration(void) {

	//
	// initialise dt, i.e. integration interval
	//
	dt=(double)mThreadProperties.timeValue/1000.;
	//
	// initialise Minion simulator
	//
	string fileName;
    fileName=mThreadProperties.configDir+"HIL-sim/"+mName+".cfg";
	FILE *f;
	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: BlueBoatHwSimThread::init_configuration - opening file %s\n",fileName.c_str());
		return(false);
		}
    if(!read_parameters(f)) {
		printf("BlueBoatHwSimThread::read_minion failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}

	//
	// set data and command name
	//
    mGnssPosData.set_name(mName);
    mGnssVelData.set_name(mName);
    mAhrsData.set_name(mName);

	mRbCmd.set_name(mName);
	return true;
}

bool BlueBoatHwSimThread::read_parameters(FILE *f) {
	//
	// read BlueBoat pose
	//
	if(!mPose.fread(f)) {
		printf("ERROR: BlueBoatHwSimThread::read_parameters - read_pose(...) failed\n");
		return(false);
	}
	char nome[64];
	//
	// read BlueBoat sampling time
	//
	fscanf(f,"%s %" SCNu64,nome,&mSamplingTime_ms);
	if(strcmp(nome,"samplingTime_ms")!=0) {
		printf("ERROR: BlueBoatHwSimThread::read_parameters - read %s while expected samplingTime_ms\n",nome);
		return(false);
		}
	//
	// read GPS model and parameters
	//
    char str1[64],str2[64];
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"gpsModel")!=0)
    {
        printf("BlueBoatHwSimThread::read_parameters gpsModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    string fileName;
    FILE *f1;
    fileName=mThreadProperties.configDir+"HIL-sim/"+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("BlueBoatHwSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mGnssSim->read_parameters(f1)) {
    	printf("BlueBoatHwSimThread::read_parameters ERROR calling mGnssSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }
    printf("BlueBoatHwSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());
	//
	// read AHRS model and parameters
	//
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"ahrsModel")!=0)
    {
        printf("BlueBoatHwSimThread::read_parameters ahrsModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    fileName=mThreadProperties.configDir+"HIL-sim/"+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("BlueBoatHwSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mAhrsSim->read_parameters(f1)) {
    	printf("BlueBoatHwSimThread::read_parameters ERROR calling mAhrsSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }
    printf("BlueBoatHwSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());

    printf("BlueBoatHwSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());

    fclose(f);
	return true;
}


void BlueBoatHwSimThread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- BlueBoatHwSimThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	uint8_t chr = 0;
	int chan = MAVLINK_COMM_0;
	mavlink_message_t msg;
	mavlink_status_t status;
	uint32_t p1,p2,p3;
	for (int i = 0; i < n; i++)
	{
		chr = (uint8_t)(cmd[i]);
		if (mavlink_parse_char(chan, chr, &msg, &status)) {
			switch (msg.msgid) {
			case MAVLINK_MSG_ID_HEARTBEAT:
				mavlink_heartbeat_t hb;
				mavlink_msg_heartbeat_decode(&msg, &hb);
				if (hb.type == MAV_TYPE_SURFACE_BOAT)
				{
					mHeartBeatFlag=true;

					printf("heartbeat -> type: %d    status: %d      mode: %d %d\n", hb.type, hb.system_status, hb.base_mode, hb.custom_mode);
				}
				break;
			case MAVLINK_MSG_ID_COMMAND_LONG:
				mavlink_command_long_t command_long;
				mavlink_msg_command_long_decode(&msg, &command_long);
//				uint32_t p1,p2,p3;
				p1 = *(uint32_t*)(&(command_long.param1));
				p2 = *(uint32_t*)(&(command_long.param2));
				p3 = *(uint32_t*)(&(command_long.param3));

				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
						command_long.command);

				if(command_long.command==MAV_CMD_DO_SET_MODE) {
					printf("MAV_CMD_DO_SET_MODE BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
							command_long.command);
					if((p1 == MAV_MODE_FLAG_CUSTOM_MODE_ENABLED)&&(p2==0x00000000)) {
						printf("BlueBoatHwSimThread::process_command SET_MODE manual\n");fflush(stdout);
						mMode="manual";
					}
				}
				else if(command_long.command==MAV_CMD_DO_SET_SERVO) {
					printf("MAV_CMD_DO_SET_SERVO BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
							command_long.command);
					if(command_long.param1==1.) {
						if(mServo1Function==DISABLED)
							mRightRpm = command_long.param2;
					}
					if(command_long.param1==3.) {
						if(mServo3Function==DISABLED)
							mLeftRpm = command_long.param2;
					}
					printf("BlueBoatHwSimThread::process_command left %.2lf right %.2lf\n",mLeftRpm,mRightRpm);
				}
				else if(command_long.command==MAV_CMD_COMPONENT_ARM_DISARM) {
					if((p1 == 0x3f800000)&&(p2==0x52cc)) {
						printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
								command_long.command);
						printf("BlueBoatHwSimThread::process_command SET_ARM arm\n");fflush(stdout);
						mArm="armed";
					}
					if((p1 == 0x0)&&(p2==0x52cc)) {
						printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
								command_long.command);
						printf("BlueBoatHwSimThread::process_command SET_ARM disarm\n");fflush(stdout);
						mArm="disarmed";
					}
				}
				else {
					printf("default BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
							command_long.command);
					fflush(stdout);
				}


////				if((p1 == 0x3f800000)&&(p2==0x00000000)) {
//				if((p1 == MAV_MODE_FLAG_CUSTOM_MODE_ENABLED)&&(p2==0x00000000)) {
//					printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
//							command_long.command);
//					printf("BlueBoatHwSimThread::process_command SET_MODE manual\n");fflush(stdout);
//					mMode="manual";
//				}
////				if((p1 == 0x3f800000)&&(p2==0x40800000)) {
////					printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
////							command_long.command);
////					printf("BlueBoatHwSimThread::process_command SET_MODE hold\n");fflush(stdout);
////					mMode="hold";
////				}
//				if((p1 == 0x3f800000)&&(p2==0x52cc)) {
//					printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
//							command_long.command);
//					printf("BlueBoatHwSimThread::process_command SET_ARM arm\n");fflush(stdout);
//					mArm="armed";
//				}
//				if((p1 == 0x0)&&(p2==0x52cc)) {
//					printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
//							command_long.command);
//					printf("BlueBoatHwSimThread::process_command SET_ARM disarm\n");fflush(stdout);
//					mArm="disarmed";
//				}
////				else {
////					printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu16 "\n",
////							command_long.command);
////					fflush(stdout);
////				}


//				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu32 " %" PRIu32 " %" PRIu32 "\n",p1,p2,p3);fflush(stdout);
//				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG float %e %e %e\n",command_long.param1,command_long.param2,command_long.param3);fflush(stdout);
//				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %x %x %x\n",p1,p2,p3);fflush(stdout);
				break;
			case MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE:
				mavlink_rc_channels_override_t rc_channels_override;
				mavlink_msg_rc_channels_override_decode(&msg, &rc_channels_override);
				uint16_t c1,c2,c3,c4;
				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 "\n",
						rc_channels_override.chan1_raw,rc_channels_override.chan2_raw,
						rc_channels_override.chan3_raw,rc_channels_override.chan4_raw);
				fflush(stdout);

				F(0) = ((double)(rc_channels_override.chan3_raw)-1500.)/4.0;
				T(2) = ((double)(rc_channels_override.chan1_raw)-1500.)/4.0;

				printf("BlueBoatHwSimThread::process_command SET_THRUST_TORQUE %lf %lf\n",F(0),T(2));
				fflush(stdout);
				break;
			case MAVLINK_MSG_ID_PARAM_SET:
				mavlink_param_set_t param_set;
				mavlink_msg_param_set_decode(&msg,&param_set);
				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_PARAM_SET %s %f\n",
						param_set.param_id,param_set.param_value);
				if(strcmp(param_set.param_id,"SERVO1_FUNCTION")==0)
					mServo1Function=param_set.param_value;
				if(strcmp(param_set.param_id,"SERVO3_FUNCTION")==0)
					mServo3Function=param_set.param_value;
				fflush(stdout);
				break;
			default:
				printf("BlueBoatHwSimThread::process_command msg.msgid %" PRIu32 "\n",msg.msgid);
//				printf("BlueBoatHwSimThread::process_command MAVLINK_MSG_ID_COMMAND_LONG %" PRIu32 " %" PRIu32 " %" PRIu32 "\n",p1,p2,p3);fflush(stdout);

				break;
			}

		}
	} // for

}


void BlueBoatHwSimThread::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;
	if(dataStrType=="LIN_POS")
		mVehPos.sread(data);
	else if(dataStrType=="ANG_POS")
		mVehAttitude.sread(data);
	else if(dataStrType=="LIN_VEL")
		mVehLinVel.sread(data);
	else if(dataStrType=="ANG_VEL")
		mVehAngVel.sread(data);
	else if(dataStrType=="FORCE_MOMENT")
		; // do nothing
	else {
		printf("BlueBoatHwSimThread::process_data unexpected data type %s\n%s\n",dataStrType.c_str(),data);
		fflush(stdout);
	}
}

void BlueBoatHwSimThread::execute_thread_semantics(void) {
//	printf("BlueBoatHwSimThread::execute_thread_semantics() %s %" PRIu64 "\n",mName.c_str(),mElapsedMilliseconds);fflush(stdout);
	//
	// compute BlueBoat pose
	//
	compute_device_pose(mBlueBoatPos,mBlueBoatAttitude,mBlueBoatLinVel,mBlueBoatAngVel,
			mPose,
			mVehPos,mVehAttitude,mVehLinVel,mVehAngVel);

	//
	// get GPS data
	//
	mGnssSim->get_measurement(mGnssPosData,mGnssVelData,mBlueBoatPos,mBlueBoatLinVel,mElapsedMilliseconds);
	//
	// get IMU data
	//
	mAhrsSim->get_measurement(mAhrsData,mBlueBoatAttitude,mBlueBoatLinVel,mBlueBoatAngVel,mElapsedMilliseconds);

	//
	// build force and torque command and append output to mCmdSendBuffer to rigid-body
	//
	build_force_torque_cmd();
}

void BlueBoatHwSimThread::build_force_torque_cmd(void) {
	if((mMode!="manual")||(mArm!="armed")) {
		F.zeros();
		T.zeros();
	}
	else {
		//
		// compute TL,TR
		//
		printf("BlueBoatHwSimThread::build_force_torque_cmd left %.2lf right %.2lf\n",mLeftRpm,mRightRpm);fflush(stdout);
		double leftPerc = (1500.-mLeftRpm)/4.;
		double rightPerc = (mRightRpm-1500.)/4.;
		double TL = compute_propeller_thrust(1.,0.3898,leftPerc/100.);
		double TR = compute_propeller_thrust(1.,0.3898,rightPerc/100.);

		double f=TL+TR;
		if(f>=0.)
			F(0)=100.*f/2.;
		else
			F(0)=100.*f/(2.*0.3898);

		T(2)=100*(TL-TR)/(2.*0.3898);

	}
	//
	// compute force & torque in the RB-fixed reference frame
	//
//	printf("BlueBoatHwSimThread::build_force_torque_cmd to be implemented\n");fflush(stdout);
//	F=R_BD*force;
//	T=p%F;
}


void BlueBoatHwSimThread::build_commands(void) {
	//
	// build packet: force & moment in the RB-fixed reference frame
	//
	mRbCmd.set(F(0), F(1), F(2), T(0), T(1), T(2));
	//
	// append packet to cmdSendBuffer
	//
	Packet packet;
	packet.n=mRbCmd.code_set_force_moment((char*)packet.data);
	mCmdSendBuffer[0].push(packet); // there is only one rigid body connected to the BlueBoat simulator
//	printf("BlueBoatHwSimThread::build_commands %s",packet.data);

}


void BlueBoatHwSimThread::build_data(void) {
//	printf("====================== BlueBoatHwSimThread::build_data %" PRIu64 "\n",mElapsedMilliseconds);fflush(stdout);

	if((mElapsedMilliseconds%mSamplingTime_ms)!=0)
		return;

	Packet packet;
	mavlink_message_t msg;
	//
	// battery voltage
	//
	mavlink_sys_status_t st;
	st.voltage_battery=1000*BATTERY_VOLTAGE_FULL;
	mavlink_msg_sys_status_encode(1, 1, &msg, &st);
	packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
	mDataSendBuffer.push(packet);
//	printf("BlueBoatHwSimThread::build_data batt %lf\n",BATTERY_VOLTAGE_FULL);fflush(stdout);
//	printf("BlueBoatHwSimThread::build_data batt %d %" PRIu64 "\n",packet.n,mElapsedMilliseconds);fflush(stdout);
	//
	// global position
	//
	mavlink_global_position_int_t gp;
	gp.lat = (int32_t)(mBlueBoatPos.latitude * 10000000);
	gp.lon = (int32_t)(mBlueBoatPos.longitude * 10000000);
	gp.vx = (int16_t)(mBlueBoatLinVel.xDot * 100);
	gp.vy = (int16_t)(mBlueBoatLinVel.yDot * 100);
	gp.hdg = (uint16_t)(mBlueBoatLinVel.course * 100);
	mavlink_msg_global_position_int_encode(1, 1, &msg, &gp);
	packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
	mDataSendBuffer.push(packet);
//	printf("BlueBoatHwSimThread::build_data gp %d %" PRIu64 "\n",packet.n,mElapsedMilliseconds);fflush(stdout);
	//
	// attitude
	//
	mavlink_attitude_t att;
	att.roll = (float)mBlueBoatAttitude.phi*M_PI/180.;
	att.pitch = (float)mBlueBoatAttitude.theta*M_PI/180.;
	att.yaw = (float)mBlueBoatAttitude.psi*M_PI/180.;
	att.rollspeed = (float)mBlueBoatAngVel.p*M_PI/180.;
	att.pitchspeed = (float)mBlueBoatAngVel.q*M_PI/180.;
	att.yawspeed = (float)mBlueBoatAngVel.r*M_PI/180.;
	mavlink_msg_attitude_encode(1, 1, &msg, &att);
	packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
	mDataSendBuffer.push(packet);
	//printf("BlueBoatHwSimThread::build_data att %d %" PRIu64 "\n",packet.n,mElapsedMilliseconds);fflush(stdout);
//	printf("BlueBoatHwSimThread::build_data att %f %f %f %f %f %f\n",
//			att.roll,att.pitch,att.yaw,att.rollspeed,att.pitchspeed,att.yawspeed);
//	fflush(stdout);
	//
	// servo output
	//
	mavlink_servo_output_raw_t srv;
	srv.servo1_raw = (uint16_t)mRightRpm;
	srv.servo3_raw = (uint16_t)mLeftRpm;
	mavlink_msg_servo_output_raw_encode(1,1,&msg,&srv);
	packet.n = mavlink_msg_to_send_buffer((uint8_t*)&packet.data[0], &msg);
	mDataSendBuffer.push(packet);

}




