/*
 * SwampActuation.h
 *
 *  Created on: 5 March 2024
 *      Author: massimo
 */
#ifndef SWAMPACTUATION_H_
#define SWAMPACTUATION_H_

#include "TimeStampedData.h"

class SwampActuation : public TimeStampedData {
public:
	//
	// Manual mode references
	//
	double alpha; // azimuth [deg]
	double n; // [rpm]
	//
	// Minion references
	//
	// Minion RL
	double alphaRL; // azimuth [deg]
	double nRL; // [rpm]
	// Minion FL
	double alphaFL; // azimuth [deg]
	double nFL; // [rpm]
	// Minion RR
	double alphaRR; // azimuth [deg]
	double nRR; // [rpm]
	// Minion FR
	double alphaFR; // azimuth [deg]
	double nFR; // [rpm]
public:
	SwampActuation(string name);
    ~SwampActuation(void) {};

    SwampActuation & operator = (const SwampActuation &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

public:
	void reset(void);
};

#endif /* SWAMPACTUATION_H_ */
