/*
 * LinearPosition.h
 *
 *  Created on: 15 December 2023
 *      Author: massimo
 */
#ifndef LINEARPOSITION_H_
#define LINEARPOSITION_H_

#include "TimeStampedData.h"
#include "nVector.h"

#define LINEAR_POSITION_CODE "LIN_POS"

class LinearPosition : public TimeStampedData {
public:
	// latitude, longitude
	double latitude; // [deg.dddddddd]
	double longitude; // [deg.dddddddd]
	// local Earth-fixed NED frame coordinates
	double lat0; // [deg.dddddddd]
	double lon0; // [deg.dddddddd]
	// position in the local Earth-fixed NED frame
	double x; // [m]
	double y; // [m]
	double z; // [m] - z is depth, positive downward

public:
	LinearPosition(string name);
	LinearPosition(void);
	LinearPosition(const LinearPosition& obj) { *this = obj; }
    ~LinearPosition(void) {};

    LinearPosition & operator = (const LinearPosition &obj);

    bool fread(FILE *f);

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

#endif /* LINEARPOSITION_H_ */
