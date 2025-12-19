/*
 * GPSdata.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
//
// Data provided by the standard NMEA sentences GGA, VTG and ZDA are included
//
#ifndef GPSDATA_H_
#define GPSDATA_H_
#include "TimeStampedData.h"
#include "MathAngles.h"

class GPSdata : public TimeStampedData {
public:
	// date, time
	uint64_t date; // yyyymmdd : date = 10000*yyyy + 100*mm + dd (ZDA, obained by...)
	double time; // hhmmss.ss in UTC (GGA, ZDA)
	// latitude, longitude
	double latitude; // [deg.dddddddd] (GGA)
	double longitude; // [deg.dddddddd] (GGA)
	// GPS height
	double heightAboveEllipsoid; // [m] (GGA: Ellipsoid altitude = MSL Altitude + Geoid Separation)
	double heightAboveMSL; // [m] (GGA)
	// speed and track
	double speed; // [m/s] (VTG, converted to [m/s])
	double track; // [deg] (VTG, obtained by...)
	// fix quality, n satellites, HDOP
	uint8_t fixQuality; // (GGA)
	uint8_t nSatellites; // GGA)
	double hdop;

public:
	GPSdata(string name);
	GPSdata(void);
    ~GPSdata(void) {};

    GPSdata & operator = (const GPSdata &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

    int sprint_NMEA_GGA(char *s);
    int sprint_NMEA_VTG(char *s);
    int sprint_NMEA_ZDA(char *s);
    double deg_dec_2_deg_min(double degDec);

private:
	void reset(void);
 };

#endif /* GPSDATA_H_ */
