/*
 * TrueHeadingData.h
 *
 *  Created on: 24 October 2023
 *      Author: massimo
 */
#ifndef TRUEHEADINGDATA_H_
#define TRUEHEADINGDATA_H_
#include "TimeStampedData.h"
#include "MathAngles.h"

class TrueHeadingData : public TimeStampedData {
public:
	// true heading
	double trueHeading; // [deg]

public:
	TrueHeadingData(string name);
	TrueHeadingData(void);
    ~TrueHeadingData(void) {};

    TrueHeadingData & operator = (const TrueHeadingData &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

    int sprint_NMEA_HDT(char *s);

private:
	void reset(void);
 };


#endif /* TRUEHEADINGDATA_H_ */
