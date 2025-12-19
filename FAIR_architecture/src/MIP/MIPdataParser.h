/*
 * MIPdataParser.h
 *
 *  Created on: Oct 17, 2023
 *      Author: Massimo Caccia
 *
 *  Note 1: this parser assumes Single Rate Data
 *  Note 2: this parser processes only packets with Descriptor Set byte equal to 0x80 (Sensor Data)
 *
 */

#ifndef MIPDATAPARSER_H_
#define MIPDATAPARSER_H_

#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#include "DataPacket.h"
#include "DataParser.h"
#include "FletcherChecksum.h"
#include "AhrsData.h"

// Sync Bytes
#define BYTE_SYNC1 0x75
#define BYTE_SYNC2 0x65
// Descriptor Sets
#define SENSOR_DATA 0x80
#define FILTER_DATA 0x82
#define SHARED_DATA 0xFF
#define SYSTEM_DATA 0xA0
// Field Descriptors : Sensor Data
#define SENSOR_DATA_SCALED_ACCEL 0x04
#define SENSOR_DATA_SCALED_GYRO 0x05
#define SENSOR_DATA_SCALED_MAG 0x06
#define SENSOR_DATA_DELTA_THETA 0x07
#define SENSOR_DATA_DELTA_VELOCITY 0x08
#define SENSOR_DATA_CF_ORIENTATION_MATRIX 0x09
#define SENSOR_DATA_CF_EULER_ANGLES 0x0C
// in recorded data it is 0x80,0xD3 instead of 0x80,0x12
#define GPS_TIME_STAMP 0xD3
// null descriptor
#define NULL_DESCRIPTOR 0x00

typedef struct {
	float accel[3];			// [g]
	float gyro[3];			// (roll,pitch,yaw) [rad/s]
	float mag[3];			// (x,y,z) [Gauss]
	float deltaTheta[3];	// (roll,pitch,yaw) [rad]
	float deltaVelocity[3];	// (x,y,z) [g*s]
	float eulerAngles[3];	// (roll,pitch,yaw) [rad]
	double gpsTimeOfWeek;	// GPS Time of Week [seconds]
	uint16_t gpsWeekNumber;	// GPS Week Number since 1980 [weeks]
	uint16_t validFlags;
} MIPdata;

class MIPdataParser : public DataParser {
public:
//	MIPdataParser(DataPacket *packetPtr);
	MIPdataParser(void);
	~MIPdataParser() {}

	void get_data(AhrsData &data) { data=mAhrsData; mAhrsData.mValid=false;}

private:
	bool detect_header(uint8_t byte);
	bool build_packet(uint8_t byte);
	bool decode_packet(void);			// decode packet data, called when process_byte(.) returns true
	void reset(void);

	float read_float(uint8_t* &uint8ptr);
	uint16_t read_uint16(uint8_t* &uint8ptr);
	uint64_t read_uint64(uint8_t* &uint8ptr);
	double read_double(uint8_t* &uint8ptr);

	void update_data(void);
	double mDt;	// sampling time: sampling frequency is set by default to 20 Hz
	void set_samplig_time(double dt) {mDt=dt;}

private:
	uint8_t mDescriptorSet;
	uint8_t mFieldDescriptor;
	uint8_t mPayloadLength;
	uint8_t mFieldLength;
	uint8_t mReadBytes;

	uint8_t mByteSync1;
	uint8_t mByteSync2;

	uint8_t *mUint8Ptr;
	uint8_t mProcessedBytes;

	FletcherChecksum mFletcherChecksum;
	uint16_t mChecksum;

	MIPdata mMIPdata;

	AhrsData mAhrsData;

	double g;	// standard acceleration of gravity
				// or standard acceleration of free fall,
				// often called simply standard gravity and denoted by ɡ0 or ɡn,
				// is the nominal gravitational acceleration of an object in a vacuum near the surface of the Earth
};

#endif /* MIPDATAPARSER_H_ */
