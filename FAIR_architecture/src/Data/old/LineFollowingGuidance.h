/*
 * LineFollowingGuidance.h
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
#ifndef LINEFOLLOWINGGUIDANCE_H_
#define LINEFOLLOWINGGUIDANCE_H_

#include "TimeStampedData.h"
#include "nVector.h"

class LineFollowingGuidance : public TimeStampedData {
public:
	// latitude, longitude of a line point
	double latL; // [deg.dddddddd]
	double lonL; // [deg.dddddddd]
	// local Earth-fixed NED frame coordinates
	double lat0; // [deg.dddddddd]
	double lon0; // [deg.dddddddd]
	// position of a line point in the local Earth-fixed NED frame
	double xL; // [m]
	double yL; // [m]
	// orientation of the line in the local Earth-fixed NED frame
	double gammaL; // [deg]
	// linear velocity in vehicle-fixed frame
	double uLF; // [m/s]
public:
	LineFollowingGuidance(string name);
	LineFollowingGuidance(void);
    ~LineFollowingGuidance(void) {};

    LineFollowingGuidance & operator = (const LineFollowingGuidance &obj);

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

#endif /* LINEFOLLOWINGGUIDANCE_H_ */
