/*
 * BlueBoatDataParser.h
 *
 *  Created on: Mar 23, 2025
 *      Author: Massimo Caccia
 *
 */


#ifndef BLUEBOATDATAPARSER_H_
#define BLUEBOATDATAPARSER_H_

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <float.h>

#include "DataPacket.h"
#include "DataParser.h"

#include "MathAngles.h"

#include "NormalisedRpm.h"
#include "AhrsData.h"
#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "BatteryData.h"

//#include "/home/pi/mavlink/generated/include/mavlink/2.0/common/mavlink.h"
#include "../../mavlink/common/mavlink.h"
//#include "mavlink.h"

#define PWM_ZERO		1510
#define PWM_MIN			1100
#define PWM_MAX			1900
//#define PWM_DELTA_100	4

// Sync Bytes
//#define BYTE_SYNC '$'
//#define COMMA ','
//#define ASTERISK '*'
#define BYTE_END '\n'

typedef struct {
	double batteryVoltage;
	bool updated;
	void zero() {
		batteryVoltage=0.;
		updated=false;
	}
} BlueBoatSysStatus;

typedef struct {
	uint64_t gnssDate; // yyyymmdd : date = 10000*yyyy + 100*mm + dd (ZDA, obtained by...)
	double gnssTime; // hhmmss.ss in UTC (GGA, ZDA)
	double gnssLatitude;
	double gnssLongitude;
	double gnssAltitude;
	double gnssSpeed;
	double gnssTrack;
	bool updated;
	void zero() {
		gnssDate=0;
		gnssTime=0.;
		gnssLatitude=0.;
		gnssLongitude=0.;
		gnssAltitude=0.;
		gnssSpeed=0.;
		gnssTrack=0.;
		updated=false;
	}
} BlueBoatGlobalPosition;

typedef struct {
	double roll;
	double pitch;
	double yaw;
	double rollRate;
	double pitchRate;
	double yawRate;
	bool updated;
	void zero() {
		roll=0.;
		pitch=0.;
		yaw=0.;
		rollRate=0.;
		pitchRate=0.;
		yawRate=0.;
		updated=false;
	}
} BlueBoatAttitude;

typedef struct {
	double normLeftRpm;
	double normRightRpm;
	bool updated;
	void zero() {
		normLeftRpm=0.;
		normRightRpm=0.;
		updated=false;
	}
} BlueBoatServoRaw;


class BlueBoatDataParser : public DataParser {
public:
	BlueBoatDataParser(void);
	~BlueBoatDataParser() {}

	void get_heartbeat(bool &flag) { flag=mHeartBeatFlag; mHeartBeatFlag=false; }
	void get_data(NormalisedRpm &normRpm) { normRpm=mNormRpm;}
//	void get_tlm_data(NormalisedRpm &normRpm) { normRpm=mNormRpm;}
	void get_data(AhrsData &data) {data=mAhrsData; mAhrsData.mUpdated=false;}
	void get_data(GnssPositionData &posData) { posData=mGnssPosData; mGnssPosData.mUpdated=false;}
	void get_data(GnssVelocityData &velData) { velData=mGnssVelData; mGnssVelData.mUpdated=false;}
	void get_data(BatteryData &data) { data=mBatteryData; mBatteryData.mUpdated=false;}

private:
	bool detect_header(uint8_t byte);
//	bool build_packet(uint8_t byte);
	bool decode_packet(void);			// decode packet data, called when process_byte(.) returns true
	void reset(void);

//	string read_terminator_string(uint8_t* &uint8ptr,const char cEnd);

//	void state_update(void);
	void speed_motor_data_update(void);
	void ahrs_data_update(void);
	void gnss_data_update(void);
	void battery_data_update(void); // { mBatteryData.voltage=mBlueBoatSysStatus.batteryVoltage; }
	void update_data(void);

	void print_heartbeat(mavlink_heartbeat_t hb);

private:
	uint32_t mavlinkMessage;

	bool mHeartBeatFlag;
	BlueBoatSysStatus mBlueBoatSysStatus;
	BlueBoatGlobalPosition mBlueBoatGlobalPosition;
	BlueBoatAttitude mBlueBoatAttitude;
	BlueBoatServoRaw mBlueBoatServoRaw;

	NormalisedRpm mNormRpm;
	AhrsData mAhrsData;
	GnssPositionData mGnssPosData;
	GnssVelocityData mGnssVelData;
	BatteryData mBatteryData;
};

#endif /* BLUEBOATDATAPARSER_H_ */
