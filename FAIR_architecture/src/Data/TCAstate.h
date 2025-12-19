/*
 * TCAstate.h
 *
 *  Created on:7 August 2024
 *      Author: massimo
 */
#ifndef TCASTATE_H_
#define TCASTATE_H_

#include "TimeStampedData.h"

#define TCA_STATE_CODE "TCA_STATE"

enum PropulsionClass {
	DIFFERENTIAL_PROPELLERS=0,
	AZIMUTH_JETS_4,
	UNDEFINED
} ;

class TCAstate : public TimeStampedData {
public:
	// TCA propulsion class
	PropulsionClass mPropulsionClass;
	// TCA enable
	bool mEnable;
	// TCA mode: "auto" / "manual" / "raw"
	string mMode;
	// TCA function;
	string mFunction; // "steering" / "hovering"
	// TCA configuration
	string mConfig; // "surge_yaw" / "diamond" / "free"

public:
	TCAstate(void);
	TCAstate(string name);
    ~TCAstate(void) {};

    TCAstate & operator = (const TCAstate &obj);

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

#endif /* TCASTATE_H_ */
