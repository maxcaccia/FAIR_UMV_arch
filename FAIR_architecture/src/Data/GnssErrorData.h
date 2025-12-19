/*
 * GnssErrorData.h
 *
 *  Created on: 19 December 2024
 *      Author: massimo
 */
//
// Data provided by the standard NMEA sentences GTS are included
//
#ifndef GNSSERRORDATA_H_
#define GNSSERRORDATA_H_
#include "TimeStampedData.h"
#include "MathAngles.h"

#define GNSS_ERROR_CODE "GNSS_ERR"

class GnssErrorData : public TimeStampedData {
public:
	// time
	double time; // hhmmss.ss in UTC (GTS)
	// RMS value of the standard deviation of the ranges
	double rangeRms; // [m] (GTS)
	// ellipse standard devation
	double stdMajor;		// [m] semi-major axis
	double stdMinor;		// [m] semi-minor axis
	double orient;			// [deg] orientation of semi-major axis
	// lat, lon, alt standard deviation
	double stdLat;			// [m]
	double stdLon;			// [m]
	double stdAlt;			// [m]

public:
	GnssErrorData(string name);
	GnssErrorData(void);
	GnssErrorData(const GnssErrorData& obj) { *this = obj; }
    ~GnssErrorData(void) {};

    GnssErrorData & operator = (const GnssErrorData &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

    int sprint_NMEA_GTS(char *s,string talkerId);

private:
	void reset(void);
 };

#endif /* GNSSERRORDATA_H_ */
