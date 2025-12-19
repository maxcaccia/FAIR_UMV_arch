/*
 * BodySpeedData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef BODYSPEEDDATA_H_
#define BODYSPEEDDATA_H_
#include "GnssVelocityData.h"

#define BODY_SPEED_CODE "BODY_SPEED"

class BodySpeedData : public TimeStampedData {
public:
	// speed, track
	double speed; // [m/s]
	double track; // [deg]
	// speed, track stdev
	double speedStd; // speed [m/s]
	double trackStd; // track [deg]

public:
	BodySpeedData(string name);
	BodySpeedData(void);
	BodySpeedData(const BodySpeedData& obj) { *this = obj; }
    ~BodySpeedData(void) {};

    BodySpeedData & operator = (const BodySpeedData &obj);
    void assign(GnssVelocityData gnssVelocityData);
    void assign(double sigmaU,double sigmaGamma) {speedStd=sigmaU;trackStd=sigmaGamma;}

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

#endif /* BODYSPEEDDATA_H_ */
