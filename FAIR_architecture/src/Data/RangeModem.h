/*
 * RangeModem.h
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
#ifndef RANGEMODEM_H_
#define RANGEMODEM_H_

#include "TimeStampedData.h"

#define RANGE_MODEM_CODE "RNG_MDM"

class RangeModem : public TimeStampedData {
public:
	// position in the local Earth-fixed NED frame
	double x; // [m]
	double y; // [m]
	double z; // [m] - z is depth, positive downward
	// estimated covariance of horizontal position
	double Pxx; // [m^2]
	double Pyy; // [m^2]
	double Pxy; // [m^2]
	// range
	double rng; // [m]

public:
	RangeModem(string name);
	RangeModem(void);
	RangeModem(const LinearPosition& obj) { *this = obj; }
    ~RangeModem(void) {};

    RangeModem & operator = (const RangeModem &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

private:
	void reset(void);

};

#endif /* RANGEMODEM_H_ */
