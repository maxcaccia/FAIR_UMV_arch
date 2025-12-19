/*
 * BodyPositionData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
//
// latitude , longitude
// lat stdev , lon stdev
//
#ifndef BODYPOSITIONDATA_H_
#define BODYPOSITIONDATA_H_
#include "GnssPositionData.h"

#define BODY_POSITION_CODE "BODY_POS"

#define GPS_FIX_STD		4.9
#define DGNSS_STD		0.4
#define RTK_FIXED_STD	0.01
#define RTK_FLOAT_STD	0.3


class BodyPositionData : public TimeStampedData {
public:
	// latitude, longitude
	double lat; // latitude [m]
	double lon; // longitude [m]
	// latitude, longitude stdev
	double latStd; // latitude [m]
	double lonStd; // longitude [m]

public:
	BodyPositionData(string name);
	BodyPositionData(void);
	BodyPositionData(const BodyPositionData& obj) { *this = obj; }
    ~BodyPositionData(void) {};

    BodyPositionData & operator = (const BodyPositionData &obj);
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
	bool compute_position_stdev(GnssPositionData gnssPositionData);
 };

#endif /* BODYPOSITIONDATA_H_ */
