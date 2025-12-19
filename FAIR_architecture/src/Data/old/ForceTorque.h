/*
 * ForceTorque.h
 *
 *  Created on: 17 December 2023
 *      Author: massimo
 */
#ifndef FORCETORQUE_H_
#define FORCETORQUE_H_

#include "TimeStampedData.h"

class ForceTorque : public TimeStampedData {
public:
	// applied force to the vehicle rigid body
	double X; // [N]
	double Y; // [N]
	double Z; // [N]
	// applied torque to the vehicle rigid body
	double K; // [N m]
	double M; // [N m]
	double N; // [N m]

public:
	ForceTorque(string name);
	ForceTorque(void);
    ~ForceTorque(void) {};

    ForceTorque & operator = (const ForceTorque &obj);

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

#endif /* FORCETORQUE_H_ */
