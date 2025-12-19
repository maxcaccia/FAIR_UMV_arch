/*
 * BatteryData.h
 *
 *  Created on: May 10, 2023
 *      Author: mc
 */

#ifndef BATTERYDATA_H_
#define BATTERYDATA_H_

#include "TimeStampedData.h"

class BatteryData: public TimeStampedData {
public:
    double voltage; // [V]

public:
	BatteryData(string name);
	BatteryData(void);
	virtual ~BatteryData();

	BatteryData & operator = (const BatteryData &obj);

	void set(double V) { voltage=V; }

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

#endif /* BATTERYDATA_H_ */
