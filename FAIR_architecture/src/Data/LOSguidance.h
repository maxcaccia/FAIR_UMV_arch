/*
 * LOSguidance.h
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
#ifndef LOSGUIDANCE_H_
#define LOSGUIDANCE_H_

#include "TimeStampedData.h"
#include "nVector.h"

class LOSguidance : public TimeStampedData {
public:
	// latitude, longitude of the target point
	double latLOS; // [deg.dddddddd]
	double lonLOS; // [deg.dddddddd]
	// local Earth-fixed NED frame coordinates
	double lat0; // [deg.dddddddd]
	double lon0; // [deg.dddddddd]
	// position of the target point in the local Earth-fixed NED frame
	double xLOS; // [m]
	double yLOS; // [m]
	// linear velocity in vehicle-fixed frame
	double uLOS; // [m/s]
public:
	LOSguidance(string name);
	LOSguidance(void);
    ~LOSguidance(void) {};

    LOSguidance & operator = (const LOSguidance &obj);

    bool fread(string fileName);

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

#endif /* LOSGUIDANCE_H_ */
