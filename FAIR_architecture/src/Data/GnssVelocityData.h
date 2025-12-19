/*
 * GnssVelocityData.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
//
// Data provided by the standard NMEA sentences VTG are included
//
#ifndef GNSSVELOCITYDATA_H_
#define GNSSVELOCITYDATA_H_
#include "TimeStampedData.h"
#include "MathAngles.h"

#define GNSS_VELOCITY_CODE "GNSS_VEL"

class GnssVelocityData : public TimeStampedData {
public:
	// speed and track
	double speed; // [m/s] (VTG, converted to [m/s])
	double track; // [deg] (VTG, obtained by...)

public:
	GnssVelocityData(string name);
	GnssVelocityData(void);
	GnssVelocityData(const GnssVelocityData& obj) { *this = obj; }
    ~GnssVelocityData(void) {};

    GnssVelocityData & operator = (const GnssVelocityData &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

    int sprint_NMEA_VTG(char *s,string talkerId);

protected:
	void reset(void);
 };

#endif /* GNSSVELOCITYDATA_H_ */
