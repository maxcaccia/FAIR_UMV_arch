/*
 * LogicalGnssVelocityData.h
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
//
// Data provided by the standard NMEA sentences GGA, and ZDA are included
//
#ifndef LOGICALGNSSVELOCITYDATA_H_
#define LOGICALGNSSVELOCITYDATA_H_
#include "GnssVelocityData.h"

#define LOGICAL_GNSS_VELOCITY_CODE "LOGICAL_GNSS_VEL"

class LogicalGnssVelocityData : public GnssVelocityData {
public:
	// speed, track
	double speedStd; // latitude [m/s]
	double trackStd; // longitude [deg]

public:
	LogicalGnssVelocityData(string name);
	LogicalGnssVelocityData(void);
	LogicalGnssVelocityData(const LogicalGnssVelocityData& obj) { *this = obj; }
    ~LogicalGnssVelocityData(void) {};

    LogicalGnssVelocityData & operator = (const LogicalGnssVelocityData &obj);
    void assign(GnssVelocityData gnssVelocityData);

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

#endif /* LOGICALGNSSVELOCITYDATA_H_ */
