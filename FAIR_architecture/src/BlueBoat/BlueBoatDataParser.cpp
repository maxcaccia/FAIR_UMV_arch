/*
 * BlueBoatDataParser.cpp
 *
 *  Created on: Mar 23, 2025
 *      Author: mc
 */

#include "BlueBoatDataParser.h"

BlueBoatDataParser::BlueBoatDataParser(void) : DataParser() {
	reset();

	mHeartBeatFlag=false;
	mBlueBoatSysStatus.zero();
	mBlueBoatGlobalPosition.zero();
	mBlueBoatAttitude.zero();
	mBlueBoatServoRaw.zero();

	mavlinkMessage=0;
}

bool BlueBoatDataParser::detect_header(uint8_t byte) {
	bool flag;
	//
	// header is assumed to be always true because UDP packet data are sent from the device
	//
	flag=true;
	if(flag) {
		//		mDataPacketPtr->append_byte(byte);
		mDataPacket.append_byte(byte);
	}
	return flag;
}

//bool BlueBoatDataParser::build_packet(uint8_t byte) {
//	mDataPacket.append_byte(byte);
//	if(byte==BYTE_END)
//		return true;
//	else
//		return false;
//}

bool BlueBoatDataParser::decode_packet(void) {
//	const char *packet=(const char *)&(mDataPacket.mData[0]);

//	printf("BlueBoatDataParser::decode_packet mDataPacket.m_n %d\n",mDataPacket.m_n); fflush(stdout);

	bool flag=false;

	uint8_t chr = 0;
	int chan = MAVLINK_COMM_0;
	mavlink_message_t msg;
	mavlink_status_t status;
	for (int i = 0; i < mDataPacket.m_n; i++)
	{
		chr = (uint8_t)(mDataPacket.mData[i]);
		if (mavlink_parse_char(chan, chr, &msg, &status))
		{
			switch (msg.msgid)
			{

			case MAVLINK_MSG_ID_HEARTBEAT:
			{
				mavlinkMessage=MAVLINK_MSG_ID_HEARTBEAT;
				mavlink_heartbeat_t hb;
				mavlink_msg_heartbeat_decode(&msg, &hb);
				if (hb.type == MAV_TYPE_SURFACE_BOAT)
				{
					mHeartBeatFlag=true;

					// called for reverse engineering
					//print_heartbeat(hb);
				}
			}
			break;
			case MAVLINK_MSG_ID_SYS_STATUS:
			{
				mavlinkMessage=MAVLINK_MSG_ID_SYS_STATUS;

				mavlink_sys_status_t st;
				mavlink_msg_sys_status_decode(&msg, &st);

				mBlueBoatSysStatus.batteryVoltage = ((double)(st.voltage_battery)) / 1000.0;
				mBlueBoatSysStatus.updated=true;
				flag=true;
				//printf("BlueBoatDataParser::decode_packet MAVLINK_MSG_ID_SYS_STATUS battery voltage: %lf\n", mBlueBoatSysStatus.batteryVoltage);
				//fflush(stdout);
			}
			break;
			case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
			{
				mavlinkMessage=MAVLINK_MSG_ID_GLOBAL_POSITION_INT;

				mavlink_global_position_int_t gp;
				mavlink_msg_global_position_int_decode(&msg, &gp);

				mBlueBoatGlobalPosition.gnssLatitude = ((double)(gp.lat)) / 10000000.0;
				mBlueBoatGlobalPosition.gnssLongitude = ((double)(gp.lon)) / 10000000.0;

				double vx,vy;
				vx = ((double)(gp.vx)) / 100.0;
				vy = ((double)(gp.vy)) / 100.0;
				mBlueBoatGlobalPosition.gnssSpeed=sqrt(vx*vx+vy*vy);
				mBlueBoatGlobalPosition.gnssTrack=atan2(vy,vx);
				mBlueBoatGlobalPosition.gnssTrack=((double)(gp.hdg)) / 100.0;
				//hw.gnss.track.value = modpi((((double)(gp.hdg)) / 100.0) * M_PI / 180.0);

				//printf("BlueBoatDataParser::decode_packet MAVLINK_MSG_ID_GLOBAL_POSITION_INT latitude: %lf longitude: %lf\n",
						//mBlueBoatGlobalPosition.gnssLatitude,mBlueBoatGlobalPosition.gnssLongitude);
				//fflush(stdout);

				mBlueBoatGlobalPosition.updated=true;
				flag=true;
			}
			break;
			case MAVLINK_MSG_ID_ATTITUDE:
			{
				mavlinkMessage=MAVLINK_MSG_ID_ATTITUDE;

				mavlink_attitude_t att;
				mavlink_msg_attitude_decode(&msg, &att);

				mBlueBoatAttitude.roll = att.roll*180./M_PI;
				mBlueBoatAttitude.pitch = att.pitch*180./M_PI;
				mBlueBoatAttitude.yaw = att.yaw*180./M_PI;
				mBlueBoatAttitude.rollRate = att.rollspeed*180./M_PI;
				mBlueBoatAttitude.pitchRate = att.pitchspeed*180./M_PI;
				mBlueBoatAttitude.yawRate = att.yawspeed*180./M_PI;

				//printf("BlueBoatDataParser::decode_packet MAVLINK_MSG_ID_ATTITUDE yaw %lf\n", mBlueBoatAttitude.yaw);
				//fflush(stdout);

				mBlueBoatAttitude.updated=true;
				flag=true;
			break;
//			case MAVLINK_MSG_ID_DISTANCE_SENSOR:
//			{
//				mavlink_distance_sensor_t dist;
//				mavlink_msg_distance_sensor_decode(&msg, &dist);
//
//
//				hw.range.value = ((double)(dist.current_distance)) / 100.0;
//
//
//				hw.range.set_updated(1);
//
//				if (dist.covariance == UINT8_MAX) hw.range.valid = 0;
//				else
//				{
//					hw.range.valid = 1;
//					hw.range.std = (0.0036 / (double(UINT8_MAX - 1))) * (double(dist.covariance));
//				}
//
//				//printf("pinger >>  %lf   %lf\n", hw.range.value, hw.range.std);
//
//				update_nav_altitude(hw, nav);
			}
			break;
			case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
			{
				mavlinkMessage=MAVLINK_MSG_ID_SERVO_OUTPUT_RAW;

				mavlink_servo_output_raw_t act;
				mavlink_msg_servo_output_raw_decode(&msg, &act);
				//printf("output  %d  %d  %d  %d\n",act.servo1_raw, act.servo2_raw, act.servo3_raw, act.servo4_raw);
				////hw.status.motorLeft.value = double(act.servo1_raw - 1500) / 5.0;
				//hw.status.motorLeft.value = int64_t(act.servo1_raw);
				//hw.status.motorLeft.valid = 1;
				////hw.status.motorRight.value = -double(act.servo3_raw - 1500) / 5.0;
				//hw.status.motorRight.value = int64_t(act.servo3_raw);
				//hw.status.motorRight.valid = 1;
				mBlueBoatServoRaw.normRightRpm = double(act.servo1_raw);
				mBlueBoatServoRaw.normLeftRpm = double(act.servo3_raw);

				//printf("BlueBoatDataParser::decode_packet MAVLINK_MSG_ID_SERVO_OUTPUT_RAW left %lf right %lf\n",
						//mBlueBoatServoRaw.normLeftRpm,mBlueBoatServoRaw.normRightRpm);
				//fflush(stdout);

				mBlueBoatServoRaw.updated=true;
				flag=true;
			}
			break;
			case MAVLINK_MSG_ID_COMMAND_ACK:
			{
				mavlinkMessage=MAVLINK_MSG_ID_COMMAND_ACK;

				mavlink_command_ack_t ack;
				mavlink_msg_command_ack_decode(&msg, &ack);
				//printf("BlueBoatDataParser::decode_packet MAVLINK_MSG_ID_COMMAND_ACK  %d  %d\n",ack.command,ack.result);

				//if (ack.command == MAV_CMD_COMPONENT_ARM_DISARM && ack.result == MAV_RESULT_ACCEPTED) { armed = true; printf("Armed!\n"); }
				//if (ack.command == MAV_CMD_DO_SET_MODE && ack.result == MAV_RESULT_ACCEPTED) { manual = true; printf("Manual mode!\n"); }
			}
			break;
			case MAVLINK_MSG_ID_SYSTEM_TIME:
			case MAVLINK_MSG_ID_GPS_RAW_INT:
			case MAVLINK_MSG_ID_RAW_IMU:
			case MAVLINK_MSG_ID_SCALED_PRESSURE:
			case MAVLINK_MSG_ID_RC_CHANNELS_SCALED:
			case MAVLINK_MSG_ID_MISSION_CURRENT:
			case MAVLINK_MSG_ID_RC_CHANNELS:
			case MAVLINK_MSG_ID_VFR_HUD:
			case MAVLINK_MSG_ID_SCALED_IMU2:
			case MAVLINK_MSG_ID_POWER_STATUS:
			case MAVLINK_MSG_ID_SCALED_IMU3:
			case MAVLINK_MSG_ID_DISTANCE_SENSOR:
			case MAVLINK_MSG_ID_BATTERY_STATUS:
			case MAVLINK_MSG_ID_VIBRATION:
			case MAVLINK_MSG_ID_NAMED_VALUE_FLOAT:
				mavlinkMessage=0;
			break;
			default:
				mavlinkMessage=0;
//				printf("BlueBoatDataParser::decode_packet mavlink_message_t msgid uint32 %" PRIu32 " not considered yet\n",
//						msg.msgid);fflush(stdout);
			break;

			}
//			printf("BlueBoatDataParser::decode_packet mavlinkMessage %" PRIu32 "\n",mavlinkMessage);
		}

	}


	return flag;
}



void BlueBoatDataParser::reset(void) {
	DataParser::reset();
}



void BlueBoatDataParser::update_data(void) {
//	printf("BlueBoatDataParser::update_data %d\n",(uint32_t) mavlinkMessage); fflush(stdout);

	switch(mavlinkMessage) {
	case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
		speed_motor_data_update();
		break;
	case MAVLINK_MSG_ID_ATTITUDE:
		ahrs_data_update();
		break;
	case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
		gnss_data_update();
		break;
	case MAVLINK_MSG_ID_SYS_STATUS:
		battery_data_update();
		break;
	default:
		break;
	}

}


void BlueBoatDataParser::speed_motor_data_update(void) {
	if(mBlueBoatServoRaw.normLeftRpm<PWM_ZERO)
		mNormRpm.mLeftRpm = (-1.0)*((mBlueBoatServoRaw.normLeftRpm-PWM_ZERO)/(PWM_ZERO-PWM_MIN))*100;
	else
		mNormRpm.mLeftRpm = (-1.0)*((mBlueBoatServoRaw.normLeftRpm-PWM_ZERO)/(PWM_MAX-PWM_ZERO))*100;
	if(mBlueBoatServoRaw.normRightRpm<PWM_ZERO)
		mNormRpm.mRightRpm = (1.0)*((mBlueBoatServoRaw.normRightRpm-PWM_ZERO)/(PWM_ZERO-PWM_MIN))*100;
	else
		mNormRpm.mRightRpm = (1.0)*((mBlueBoatServoRaw.normRightRpm-PWM_ZERO)/(PWM_MAX-PWM_ZERO))*100;
		
	//mNormRpm.mLeftRpm = (-1.0)*(mBlueBoatServoRaw.normLeftRpm-PWM_ZERO)/PWM_DELTA_100;
	//mNormRpm.mRightRpm = (1.0)*(mBlueBoatServoRaw.normRightRpm-PWM_ZERO)/PWM_DELTA_100;
	mNormRpm.mUpdated = mBlueBoatServoRaw.updated;

	mBlueBoatServoRaw.updated = false;
}

void BlueBoatDataParser::ahrs_data_update(void) {
	mAhrsData.phi=mBlueBoatAttitude.roll;
	mAhrsData.theta=mBlueBoatAttitude.pitch;
	mAhrsData.psi=mBlueBoatAttitude.yaw;
	mAhrsData.p=mBlueBoatAttitude.rollRate;
	mAhrsData.q=mBlueBoatAttitude.pitchRate;
	mAhrsData.r=mBlueBoatAttitude.yawRate;
	mAhrsData.mUpdated=mBlueBoatAttitude.updated;
	mBlueBoatAttitude.updated = false;

//	printf("BlueBoatDataParser::ahrs_data_update %d %d\n",(int)mAhrsData.mUpdated,(int)mBlueBoatAttitude.updated);fflush(stdout);
}

void BlueBoatDataParser::gnss_data_update(void) {
	mGnssPosData.date=mBlueBoatGlobalPosition.gnssDate;
	mGnssPosData.time=mBlueBoatGlobalPosition.gnssTime;
	mGnssPosData.latitude=mBlueBoatGlobalPosition.gnssLatitude;
	mGnssPosData.longitude=mBlueBoatGlobalPosition.gnssLongitude;
	mGnssPosData.heightAboveEllipsoid=0.;
	mGnssPosData.heightAboveMSL=mBlueBoatGlobalPosition.gnssAltitude;
	mGnssPosData.fixQuality=1;
	mGnssPosData.nSatellites=0;
	mGnssPosData.hdop=2.;
	mGnssPosData.mUpdated=mBlueBoatGlobalPosition.updated;
	mGnssVelData.speed=mBlueBoatGlobalPosition.gnssSpeed;
	mGnssVelData.track=mBlueBoatGlobalPosition.gnssTrack;
	mGnssVelData.mUpdated=mBlueBoatGlobalPosition.updated;
	mBlueBoatGlobalPosition.updated=false;
}

void BlueBoatDataParser::battery_data_update(void) {
	mBatteryData.voltage=mBlueBoatSysStatus.batteryVoltage;
	mBatteryData.mUpdated=mBlueBoatSysStatus.updated;
	mBlueBoatSysStatus.updated=false;
}

void BlueBoatDataParser::print_heartbeat(mavlink_heartbeat_t hb) {
	printf("\nBlueBoatDataParser::print_heartbeat\n");
	printf("\ttype %" PRIu8 "\n",hb.type);
	printf("\tautopilot %" PRIu8 "\n",hb.autopilot);
	printf("\tbase_mode %" PRIu8 "\n",hb.base_mode);
	printf("\tsystem_status %" PRIu8 "\n",hb.system_status);
}



