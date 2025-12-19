/*
 * NormalisedForceMoment.h
 *
 *  Created on: 25 March 2025
 *      Author: massimo
 */
#ifndef NORMALISEDFORCEMOMENT_H_
#define NORMALISEDFORCEMOMENT_H_

#include "TimeStampedData.h"

#define NORMALISED_FORCE_MOMENT_CODE "FNORM_ORCE_MOMENT"

class NormalisedForceMoment : public TimeStampedData {
public:
	// applied force to the vehicle rigid body
	double X; // [%]
	// applied moment to the vehicle rigid body
	double N; // [%]

public:
	NormalisedForceMoment(string name);
	NormalisedForceMoment(void);
	NormalisedForceMoment(const NormalisedForceMoment& obj) { *this = obj; }
    ~NormalisedForceMoment(void) {};

    NormalisedForceMoment & operator = (const NormalisedForceMoment &obj);

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

#endif /* NORMALISEDFORCEMOMENT_H_ */
