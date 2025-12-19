/*
 * MinionDataParser.cpp
 *
 *  Created on: Feb 5, 2024
 *      Author: mc
 */

#include "MinionDataParser.h"

MinionDataParser::MinionDataParser(void) : DataParser() {
	reset();

	mMinionState.zero();
}

bool MinionDataParser::detect_header(uint8_t byte) {
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

bool MinionDataParser::build_packet(uint8_t byte) {
	mDataPacket.append_byte(byte);
	if(byte==BYTE_END)
		return true;
	else
		return false;
}

bool MinionDataParser::decode_packet(void) {
	const char *packet=(const char *)&(mDataPacket.mData[0]);

	sscanf(packet,"%d %" SCNu32 " %" SCNu32 " %" SCNu16 " %" SCNu16
			" %lf %lf %lf %lf %"
			SCNd32 " %" SCNu16 " %" SCNu32 " %" SCNd16
			" %" SCNd32 " %lf %lf %lf %lf %lf %lf %lf %"
			SCNu8 " %" SCNu32 "%lf %lf %lf %"
			SCNu8 " %" SCNu8 "%lf %lf %lf",
			&mMinionState.nodeId,&mMinionState.dateAndTime,&mMinionState.timeMs,&mMinionState.digitalOutput,&mMinionState.digitalInput,
			&mMinionState.batteryVoltage,&mMinionState.thrustMotorTemperature,&mMinionState.thrustMotorSpeed,&mMinionState.thrustMotorCurrent,
			&mMinionState.azimuthMotorPosition,&mMinionState.azimuthMotorConfigurationStatus,&mMinionState.azimuthMotorOperationStatus,&mMinionState.azimuthMotorTemperature,
			&mMinionState.azimuthMotorCurrent,&mMinionState.imuYaw,&mMinionState.imuPitch,&mMinionState.imuRoll,&mMinionState.imuXGyro,&mMinionState.imuYGyro,&mMinionState.imuZGyro,&mMinionState.imuTemperature,
			&mMinionState.imuCalibrationStatus,&mMinionState.gpsDate,&mMinionState.gpsTime,&mMinionState.gpsLatitude,&mMinionState.gpsLongitude,
			&mMinionState.gpsFixQuality,&mMinionState.gpsNSatellites,&mMinionState.gpsHDOP,&mMinionState.gpsAltitude,&mMinionState.gpsHeightGeoid
	);
	return true;
}



void MinionDataParser::reset(void) {
	DataParser::reset();
}



void MinionDataParser::update_data(void) {
	state_update();
	position_motor_tlm_data_update();
	speed_motor_tlm_data_update();
	ahrs_data_update();
	gps_data_update();
	battery_data_update();
}

void MinionDataParser::state_update(void) {
	// nopCounter is updated at each loop and zeroed when the corresponding message is updated
	mMinionState.nopCounter++;

	mMinionState.thrustMotorFault = ((mMinionState.digitalInput >> DI_THRUST_MOTOR_FAULT) & 0x0001);
	mMinionState.thrustMotorPower = ((mMinionState.digitalOutput >> DO_THRUST_MOTOR_POWER) & 0x0001);
	mMinionState.thrustMotorEnable = ((mMinionState.digitalOutput >> DO_THRUST_MOTOR_ENABLE) & 0x0001);

	mMinionState.thrustMotorSpeed=mMinionState.thrustMotorSpeed*THRUST_SPEED_PERC_FACTOR;
	//printf("Minion::state_update thrustMotorSpeed %lf %lf\n",msg.thrustMotorSpeed,minion.mMinionState.thrustMotorSpeed.value);
	if(mMinionState.thrustMotorPower==0) mMinionState.thrustMotorSpeed=0;

	//mMinionState.thrustMotorCurrent=mMinionState.thrustMotorCurrent;
	if(mMinionState.thrustMotorPower==0) mMinionState.thrustMotorCurrent=0;

	mMinionState.azimuthMotorFault = ((mMinionState.digitalInput >> DI_AZIMUTHAL_MOTOR_FAULT) & 0x0001);
	mMinionState.azimuthMotorPower = ((mMinionState.digitalOutput >> DO_AZIMUTHAL_MOTOR_POWER) & 0x0001);

	//
	// azimuthMotorEnable to be done!!!!!!!!
	//
	if((mMinionState.azimuthMotorConfigurationStatus & 0x0400) == 0x0400)
		mMinionState.azimuthMotorEnable=1;
	else
		mMinionState.azimuthMotorEnable=0;

	mMinionState.azimuthMotorAngle=wrap180(360.*mMinionState.azimuthMotorPosition/TICKS_PER_ROUND); // deg
}

void MinionDataParser::position_motor_tlm_data_update(void) {
	mPositionMotorData.position=mMinionState.azimuthMotorPosition;
	mPositionMotorData.positionDeg=mMinionState.azimuthMotorAngle;
	//
	// insert conversion if needed
	//
	mPositionMotorData.temperature=(double)mMinionState.azimuthMotorTemperature;
	//
	// insert conversion if needed
	//
	mPositionMotorData.current=(double)mMinionState.azimuthMotorCurrent;
	mPositionMotorData.fault=mMinionState.azimuthMotorFault;
	mPositionMotorData.power=mMinionState.azimuthMotorPower;
	mPositionMotorData.enable=mMinionState.azimuthMotorEnable;

//	mPositionMotorData.powerCmd=mMinionCmd.azimuthMotorPower;
//	mPositionMotorData.enableCmd=mMinionCmd.azimuthMotorEnable;
//	mPositionMotorData.positionCmd=mMinionCmd.azimuthMotorPosition;
//	mPositionMotorData.positionDegCmd=mMinionCmd.azimuthMotorAngle;
}

void MinionDataParser::speed_motor_tlm_data_update(void) {
	mSpeedMotorData.speed=mMinionState.thrustMotorSpeed;
	//
	// insert conversion if needed
	//
	mSpeedMotorData.current=mMinionState.thrustMotorCurrent;
	//
	// insert conversion if needed
	//
	mSpeedMotorData.temperature=(double)mMinionState.thrustMotorTemperature;
	mSpeedMotorData.fault=mMinionState.thrustMotorFault;
	mSpeedMotorData.power=mMinionState.thrustMotorPower;
	mSpeedMotorData.enable=mMinionState.thrustMotorEnable;
}

void MinionDataParser::ahrs_data_update(void) {
	mAhrsData.phi=mMinionState.imuRoll;
	mAhrsData.theta=mMinionState.imuPitch;
	mAhrsData.psi=mMinionState.imuYaw;
	mAhrsData.p=mMinionState.imuXGyro;
	mAhrsData.q=mMinionState.imuYGyro;
	mAhrsData.r=mMinionState.imuZGyro;
}

void MinionDataParser::gps_data_update(void) {
	mGnssPosData.date=mMinionState.gpsDate;
	mGnssPosData.time=mMinionState.gpsTime;
	mGnssPosData.latitude=mMinionState.gpsLatitude;
	mGnssPosData.longitude=mMinionState.gpsLongitude;
	mGnssPosData.heightAboveEllipsoid=mMinionState.gpsHeightGeoid;
	mGnssPosData.heightAboveMSL=mMinionState.gpsAltitude;
	mGnssPosData.fixQuality=mMinionState.gpsFixQuality;
	mGnssPosData.nSatellites=mMinionState.gpsNSatellites;
	mGnssPosData.hdop=mMinionState.gpsHDOP;
}

void MinionDataParser::get_tlm_data(PositionMotorData &data) {
	data.position=mPositionMotorData.position;
	data.positionDeg=mPositionMotorData.positionDeg;
	data.temperature=mPositionMotorData.temperature;
	data.current=mPositionMotorData.current;
	data.fault=mPositionMotorData.fault;
	data.power=mPositionMotorData.power;
	data.enable=mPositionMotorData.enable;
}

void MinionDataParser::get_tlm_data(SpeedMotorData &data) {
	data.speed=mSpeedMotorData.speed;
	data.temperature=mSpeedMotorData.temperature;
	data.current=mSpeedMotorData.current;
	data.fault=mSpeedMotorData.fault;
	data.power=mSpeedMotorData.power;
	data.enable=mSpeedMotorData.enable;
}




