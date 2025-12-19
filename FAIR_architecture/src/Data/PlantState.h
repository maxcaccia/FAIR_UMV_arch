/*
 * PlantState.h
 *
 *  Created on: 27 February 2025
 *      Author: massimo
 */
#ifndef PLANTSTATE_H_
#define PLANTSTATE_H_

#include "TimeStampedData.h"

#define PLANT_STATE_CODE "PLANT_STATE"

#define N_ACTUATORS		16
#define N_SENSORS		16
#define N_DEVICES		16

class PlantState : public TimeStampedData {
public:
	int nActuators;
	int nSensors;
	int nDevices;
	// enable command
	bool mActuatorEnableCmd[N_ACTUATORS];
	bool mSensorEnableCmd[N_SENSORS];
	bool mDeviceEnableCmd[N_DEVICES];
	// power command
	bool mActuatorPowerCmd[N_ACTUATORS];
	bool mSensorPowerCmd[N_SENSORS];
	bool mDevicePowerCmd[N_DEVICES];
	// enable data
	bool mActuatorEnableData[N_ACTUATORS];
	bool mSensorEnableData[N_SENSORS];
	bool mDeviceEnableData[N_DEVICES];
	// power data
	bool mActuatorPowerData[N_ACTUATORS];
	bool mSensorPowerData[N_SENSORS];
	bool mDevicePowerData[N_DEVICES];


public:
	PlantState(void);
	PlantState(string name);
    ~PlantState(void) {};

    PlantState & operator = (const PlantState &obj);

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

#endif /* PLANTSTATE_H_ */
