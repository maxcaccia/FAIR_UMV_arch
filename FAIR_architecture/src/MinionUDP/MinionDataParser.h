/*
 * MinionDataParser.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Massimo Caccia
 *
 */


#ifndef MINIONDATAPARSER_H_
#define MINIONDATAPARSER_H_

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <float.h>

#include "DataPacket.h"
#include "DataParser.h"

#include "MinionDriverInterface.h"

#include "MathAngles.h"

#include "PositionMotorData.h"
#include "SpeedMotorData.h"
#include "AhrsData.h"
#include "GnssPositionData.h"
#include "BatteryData.h"

// Sync Bytes
//#define BYTE_SYNC '$'
//#define COMMA ','
//#define ASTERISK '*'
#define BYTE_END '\n'

typedef struct {
	int32_t nodeId;
	uint32_t dateAndTime;
	uint32_t timeMs; //
	uint16_t digitalInput;
	uint16_t digitalOutput;
    double batteryVoltage;
    uint64_t nopCounter;

	uint8_t thrustMotorFault;
	uint8_t thrustMotorPower;
	uint8_t thrustMotorEnable;
    double thrustMotorTemperature;
    double thrustMotorSpeed;
	double thrustMotorCurrent;

	uint8_t azimuthMotorFault;
	uint8_t azimuthMotorPower;
	uint8_t azimuthMotorEnable;
    int32_t azimuthMotorPosition; // ticks
    double azimuthMotorAngle; // deg
    uint16_t azimuthMotorConfigurationStatus;
    uint32_t azimuthMotorOperationStatus;
    int16_t azimuthMotorTemperature;
    int32_t azimuthMotorCurrent;

    double imuYaw;
    double imuPitch;
    double imuRoll;
    double imuXGyro;
    double imuYGyro;
    double imuZGyro;
    double imuTemperature;
    uint8_t imuCalibrationStatus;

    uint32_t gpsDate;
    double gpsTime; // hhmmss.ss in UTC
    double gpsLatitude;
    double gpsLongitude; // tenths of degree
    uint8_t gpsFixQuality;
    uint8_t gpsNSatellites;
    double gpsHDOP;
    double gpsAltitude;
    double gpsHeightGeoid;
	void zero()
	{
		nodeId=0;
		dateAndTime=0;
		timeMs=0;
		digitalInput=0;
		digitalOutput=0;
		batteryVoltage=0;
		nopCounter=0;

		thrustMotorFault=0;
		thrustMotorPower=0;
		thrustMotorEnable=0;
	    thrustMotorTemperature=0;
	    thrustMotorSpeed=0;
	    thrustMotorCurrent=0;

		azimuthMotorFault=0;
		azimuthMotorPower=0;
		azimuthMotorEnable=0;
		azimuthMotorPosition=0;
	    azimuthMotorAngle=0;
	    azimuthMotorConfigurationStatus=0;
	    azimuthMotorOperationStatus=0;
	    azimuthMotorTemperature=0;
	    azimuthMotorCurrent=0;

	    imuYaw=0;
	    imuPitch=0;
	    imuRoll=0;
	    imuXGyro=0;
	    imuYGyro=0;
	    imuZGyro=0;
	    imuTemperature=0;
	    imuCalibrationStatus=0;

	    gpsDate=0;
	    gpsTime=0; // hhmmss.ss in UTC
	    gpsLatitude=0;
	    gpsLongitude=0; // tenths of degree
	    gpsFixQuality=0;
	    gpsNSatellites=0;
	    gpsHDOP=0;
	    gpsAltitude=0;
	    gpsHeightGeoid=0;
	}
} MinionState;


class MinionDataParser : public DataParser {
public:
	MinionDataParser(void);
	~MinionDataParser() {}

	void get_tlm_data(PositionMotorData &data); //{data=mPositionMotorData; }
	void get_tlm_data(SpeedMotorData &data); // { data=mSpeedMotorData; }
	void get_data(AhrsData &data) {data=mAhrsData; }
	void get_data(GnssPositionData &posData) { posData=mGnssPosData; }
	void get_data(BatteryData &data) { data=mBatteryData; }

private:
	bool detect_header(uint8_t byte);
	bool build_packet(uint8_t byte);
	bool decode_packet(void);			// decode packet data, called when process_byte(.) returns true
	void reset(void);

//	string read_terminator_string(uint8_t* &uint8ptr,const char cEnd);

	void state_update(void);
	void position_motor_tlm_data_update(void);
	void speed_motor_tlm_data_update(void);
	void ahrs_data_update(void);
	void gps_data_update(void);
	void battery_data_update(void) { mBatteryData.voltage=mMinionState.batteryVoltage; }
	void update_data(void);

private:
	MinionState mMinionState;

	PositionMotorData mPositionMotorData;
	SpeedMotorData mSpeedMotorData;
	AhrsData mAhrsData;
	GnssPositionData mGnssPosData;
	BatteryData mBatteryData;
};

#endif /* MINIONDATAPARSER_H_ */
