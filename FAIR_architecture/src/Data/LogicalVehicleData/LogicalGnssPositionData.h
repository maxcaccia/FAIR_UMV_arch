/*
 * LogicalGnssPositionData.h
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
//
// Data provided by the standard NMEA sentences GGA, and ZDA are included
//
#ifndef LOGICALGPSPOSITIONDATA_H_
#define LOGICALGPSPOSITIONDATA_H_
#include "GnssPositionData.h"

#define LOGICAL_GNSS_POSITION_CODE "LOGICAL_GNSS_POS"

class LogicalGnssPositionData : public GnssPositionData {
public:
	// latitude, longitude
	double latStd; // latitude [m]
	double lonStd; // longitude [m]

public:
	LogicalGnssPositionData(string name);
	LogicalGnssPositionData(void);
	LogicalGnssPositionData(const LogicalGnssPositionData& obj) { *this = obj; }
    ~LogicalGnssPositionData(void) {};

    LogicalGnssPositionData & operator = (const LogicalGnssPositionData &obj);
    void assign(GnssPositionData gnssPositionData);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

protected:
	void reset(void);
 };

#endif /* GPSPOSITIONDATA_H_ */
