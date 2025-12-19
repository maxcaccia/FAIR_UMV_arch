/*
 * PositionMotorData.h
 *
 *  Created on: 03 March 2023
 *      Author: massimo
 */
#ifndef POSITIONMOTORDATA_H_
#define POSITIONMOTORDATA_H_
#include "TimeStampedData.h"

class PositionMotorData : public TimeStampedData {
public:
	// position
	int32_t position; // [ticks]: position in ticks
	double positionDeg; // [deg] : position in deg
	// temperature
	double temperature; // [C]
	// current
	double current; // [mA]
	// fault
	uint8_t fault;
	// power
	uint8_t power;
	// enable
	uint8_t enable;
	// status
	uint16_t configurationStatus;
	uint32_t operationStatus;
	//
	// references
	//
	// reference power
	uint8_t powerCmd;
	// reference enable
	uint8_t enableCmd;
	// reference position
	int32_t positionCmd; // [ticks]: position in ticks
	double positionDegCmd; // [deg] : position in deg


public:
	PositionMotorData(string name);
	PositionMotorData(void);
    ~PositionMotorData(void) {};

    PositionMotorData & operator = (const PositionMotorData &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

private:
	void reset(void);
 };

#endif /* POSITIONMOTORDATA_H_ */
