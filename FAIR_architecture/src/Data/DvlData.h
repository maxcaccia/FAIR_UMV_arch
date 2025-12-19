/*
 * DvlData.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
#ifndef DVLDATA_H_
#define DVLDATA_H_
#include "TimeStampedData.h"
#include "MathAngles.h"

#define DVL_CODE "DVL"

class DvlData : public TimeStampedData {
public:
	// velocity
	double u; // [m/s]
	double v; // [m/s]
	double w; // [m/s]
	// range from bottom
	double rho0; // [m]
	double rho1; // [m]
	double rho2; // [m]
	double rho3; // [m]

public:
	DvlData(string name);
	DvlData(void);
	DvlData(const DvlData& obj) { *this = obj; }
    ~DvlData(void) {};

    DvlData & operator = (const DvlData &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
    int sprint(char *s);
    void sprint_csv(char *s);
    // read data
    void sread(char *s);

protected:
	void reset(void);
 };

#endif /* DVLDATA_H_ */
