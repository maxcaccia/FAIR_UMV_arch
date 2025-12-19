/*
 * ForceMoment.h
 *
 *  Created on: 17 December 2023
 *      Author: massimo
 */
#ifndef FORCEMOMENT_H_
#define FORCEMOMENT_H_

#include "TimeStampedData.h"

#define FORCE_MOMENT_CODE "FORCE_MOMENT"

class ForceMoment : public TimeStampedData {
public:
	// applied force to the vehicle rigid body
	double X; // [N]
	double Y; // [N]
	double Z; // [N]
	// applied moment to the vehicle rigid body
	double K; // [N m]
	double M; // [N m]
	double N; // [N m]

public:
	ForceMoment(string name);
	ForceMoment(void);
	ForceMoment(const ForceMoment& obj) { *this = obj; }
    ~ForceMoment(void) {};

    ForceMoment & operator = (const ForceMoment &obj);

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

#endif /* FORCEMOMENT_H_ */
