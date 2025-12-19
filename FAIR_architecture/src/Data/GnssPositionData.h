/*
 * GnssPositionData.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
//
// Data provided by the standard NMEA sentences GGA, and ZDA are included
//
#ifndef GPSPOSITIONDATA_H_
#define GPSPOSITIONDATA_H_
#include "TimeStampedData.h"
#include "MathAngles.h"

#define GNSS_POSITION_CODE "GNSS_POS"

class GnssPositionData : public TimeStampedData {
public:
	// date, time
	uint64_t date; // yyyymmdd : date = 10000*yyyy + 100*mm + dd (ZDA, obtained by...)
	double time; // hhmmss.ss in UTC (GGA, ZDA)
	// latitude, longitude
	double latitude; // [deg.dddddddd] (GGA)
	double longitude; // [deg.dddddddd] (GGA)
	// GPS height
	double heightAboveEllipsoid; // [m] (GGA: Ellipsoid altitude = MSL Altitude + Geoid Separation)
	double heightAboveMSL; // [m] (GGA)
	// fix quality, n satellites, HDOP
	uint8_t fixQuality; // (GGA)
	uint8_t nSatellites; // GGA)
	double hdop; // (GGA)

public:
	GnssPositionData(string name);
	GnssPositionData(void);
	GnssPositionData(const GnssPositionData& obj) { *this = obj; }
    ~GnssPositionData(void) {};

    GnssPositionData & operator = (const GnssPositionData &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

    int sprint_NMEA_GGA(char *s,string talkerId);
    int sprint_NMEA_ZDA(char *s,string talkerId);
    double deg_dec_2_deg_min(double degDec);

protected:
	void reset(void);
 };

#endif /* GPSPOSITIONDATA_H_ */
