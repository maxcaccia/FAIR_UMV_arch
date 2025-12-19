/*
 * NormalisedForceMoment.h
 *
 *  Created on: 26 March 2025
 *      Author: massimo
 */
#ifndef NORMALISEDRPM_H_
#define NORMALISEDRPM_H_

#include "TimeStampedData.h"

#define NORMALISED_RPM_CODE "NORM_RPM"

class NormalisedRpm : public TimeStampedData {
public:
	// normalised left rpm [%]
	double mLeftRpm; // [%]
	// normalised right rpm [%
	double mRightRpm; // [%]

public:
	NormalisedRpm(string name);
	NormalisedRpm(void);
	NormalisedRpm(const NormalisedRpm& obj) { *this = obj; }
    ~NormalisedRpm(void) {};

    NormalisedRpm & operator = (const NormalisedRpm &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

//private:
	void reset(void);
};

#endif /* NORMALISEDRPM_H_ */
