/*
 * SpeedMotorData.h
 *
 *  Created on: 02 March 2023
 *      Author: massimo
 */
#ifndef SPEEDMOTORDATA_H_
#define SPEEDMOTORDATA_H_
#include "TimeStampedData.h"

#define SPEED_MOTOR_CODE "VEL_MOT"

class SpeedMotorData : public TimeStampedData {
public:
	// speed
	double speed; // [rpm]
	// current
	double current; // [A]
	// temperature
	double temperature; // [C]

	// fault / power / enable
	uint8_t fault;
	uint8_t power;
	uint8_t enable;
	//
	// references
	//
	uint8_t powerCmd;
	uint8_t enableCmd;
	double speedCmd; // [rpm]

public:
	SpeedMotorData(string name);
	SpeedMotorData(void);
    ~SpeedMotorData(void) {};

    SpeedMotorData & operator = (const SpeedMotorData &obj);

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

#endif /* SPEEDMOTORDATA_H_ */
