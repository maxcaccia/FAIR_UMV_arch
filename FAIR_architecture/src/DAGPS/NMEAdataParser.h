/*
 * NMEAdataParser.h
 *
 *  Created on: Oct 23, 2023
 *      Author: Massimo Caccia
 *
 *  Note: this parser decodes GGA, VTG, ZDA, HDT messages
 *
 */


#ifndef NMEADATAPARSER_H_
#define NMEADATAPARSER_H_

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <float.h>

#include "DataPacket.h"
#include "DataParser.h"

#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "GnssErrorData.h"
#include "TrueHeadingData.h"

#include "MathAngles.h"

// Sync Bytes
#define BYTE_SYNC '$'
#define COMMA ','
#define ASTERISK '*'
#define BYTE_END '\n'

// NMEA messages
#define GGA 0x00
#define VTG 0x01
#define ZDA 0x02
#define HDT 0x03
// null message
#define NULL_MESSAGE 0xFF

typedef struct {
	double time;			// hhmmss.ss in UTC
	double latitude;		// [degmin.dddddddd]
	double longitude;		// [degmin.dddddddd]
	uint8_t fixQuality;		// position fix indicator
	uint8_t nSatellites;	// satellites used
	double hdop;			// HDOP
	double heightAboveMSL;	// MSL altitude [m]
	double geoidSeparation;	// Geoid-to-ellipsoid separation. Ellipsoid altitude = MSL Altitude + Geoid Separation
	double aoDGPS;			// Age of differential GPS data record
	uint16_t dgpsStationId;	// Diff. Ref. Station ID
} GGAdata;

typedef struct {
	double trackT;			// true track [deg]
	double trackM;			// magnetic track [deg]
	double speedKnots;		// speed [knots]
	double speedKmH;		// speed [Km/h]
	uint8_t mode;			// mode indicator
} VTGdata;

typedef struct {
	double time;			// hhmmss.ss in UTC
	uint8_t day;			// day
	uint8_t month;			// month
	uint16_t year;			// year
	int8_t localZoneHour;	// Local time zone offset from GMT, ranging from 00 through ±13 hours
	uint8_t localZoneMinute;// Local time zone offset from GMT, ranging from 00 through 59 minutes
} ZDAdata;

typedef struct {
	double headingT;			// true heading [deg]
} HDTdata;

typedef struct {
	double time;			// hhmmss.ss in UTC
	double rangeRms;		// [m] RMS value of the standard deviation of the ranges
	double stdMajor;		// [m] Standard deviation of semi-major axis
	double stdMinor;		// [m] Standard deviation of semi-minor axis
	double orient;			// [deg] Orientation of semi-major axis
	double stdLat;			// [m] Standard deviation of latitude error
	double stdLon;			// [m] Standard deviation of longitude error
	double stdAlt;			// [m] Standard deviation of altitude error
} GTSdata;


class NMEAdataParser : public DataParser {
public:
//	NMEAdataParser(DataPacket *packetPtr);
	NMEAdataParser(void);
	~NMEAdataParser() {}

	void get_data(GnssPositionData &posData,GnssVelocityData &velData) { posData=mGnssPosData; velData=mGnssVelData; mGnssPosData.mValid=false; mGnssVelData.mValid=false;}
	void get_data(TrueHeadingData &data) {data=mTrueHeadingData; mTrueHeadingData.mValid=false;}
	void get_data(GnssPositionData &posData,GnssVelocityData &velData,GnssErrorData &gnssErrorData)
		{ posData=mGnssPosData; velData=mGnssVelData; gnssErrorData=mGnssErrorData; mGnssPosData.mValid=false; mGnssVelData.mValid=false; mGnssErrorData.mValid=false;}

private:
	bool detect_header(uint8_t byte);
	bool build_packet(uint8_t byte);
	bool decode_packet(void);			// decode packet data, called when process_byte(.) returns true
	void reset(void);

	bool decode_GGA(uint8_t* &uint8ptr);
	bool decode_VTG(uint8_t* &uint8ptr);
	bool decode_ZDA(uint8_t* &uint8ptr);
	bool decode_HDT(uint8_t* &uint8ptr);
	bool decode_GTS(uint8_t* &uint8ptr);

	string read_terminator_string(uint8_t* &uint8ptr,const char cEnd);

	double deg_min_2_deg_dec(double degMin);

	void update_data(void);

private:
	uint8_t mByteSync;

	uint8_t *mUint8Ptr;
	uint8_t mNMEAmessage;

	uint8_t mChecksum;
	uint8_t mNMEAchecksum;

	GGAdata mGGAdata;
	VTGdata mVTGdata;
	ZDAdata mZDAdata;
	HDTdata mHDTdata;
	GTSdata mGTSdata;

	GnssPositionData mGnssPosData;
	GnssVelocityData mGnssVelData;
	TrueHeadingData mTrueHeadingData;
	GnssErrorData mGnssErrorData;
};

//void build_GGA_packet(uint8_t *packet,GnssPositionData gnssData);
//void build_VTG_packet(uint8_t *packet,GnssVelocityData gnssData);
//void build_ZDA_packet(uint8_t *packet,GnssPositionData gnssData);
//void build_HDT_packet(uint8_t *packet,GPSdata gpsData);


#endif /* NMEADATAPARSER_H_ */
