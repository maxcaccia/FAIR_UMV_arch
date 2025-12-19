/*
 * SwampTlm.h
 *
 *  Created on: 20 December 2023
 *      Author: massimo
 */
#ifndef SWAMPTLM_H_
#define SWAMPTLM_H_

#include "TimeStampedData.h"

class SwampTlm : public TimeStampedData {
public:
	// latitude, longitude
	double latitude; // [deg.dddddddd]
	double longitude; // [deg.dddddddd]
	// angles
	double phi; // roll [deg]
	double theta; // pitch [deg]
	double psi; // yaw [deg]
	// linear velocity and speed (NED)
	double speed; // [m/s]
	double groundSpeed; // [m/s]
	double track; // [deg]
	// angular velocity
	double p; // roll rate [deg/s]
	double q; // pitch rate [deg/s]
	double r; // yaw rate [deg/s]
	// Minion FL
	double alphaFL; // azimuth [deg]
	double nRefFL; // [rpm]
	double nFL; // [rpm]
	double currentFL;
	// Minion FR
	double alphaFR; // azimuth [deg]
	double nRefFR; // [rpm]
	double nFR; // [rpm]
	double currentFR;
	// Minion RR
	double alphaRR; // azimuth [deg]
	double nRefRR; // [rpm]
	double nRR; // [rpm]
	double currentRR;
	// Minion FR
	double alphaRL; // azimuth [deg]
	double nRefRL; // [rpm]
	double nRL; // [rpm]
	double currentRL;
public:
	SwampTlm(string name);
	SwampTlm(void);
    ~SwampTlm(void) {};

    SwampTlm & operator = (const SwampTlm &obj);

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

#endif /* SWAMPTLM_H_ */
