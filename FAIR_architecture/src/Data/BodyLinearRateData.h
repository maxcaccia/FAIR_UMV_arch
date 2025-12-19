/*
 * BodyLinearRateData.h
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
#ifndef BODYLINEARRATEDATA_H_
#define BODYLINEARRATEDATA_H_

#include "DvlData.h"
#include "AhrsData.h"

#define BODY_LINEAR_RATE_CODE "BODY_LIN_VEL"

class BodyLinearRateData : public TimeStampedData {
public:
	// velocity
	double u; // [m/s]
	double v; // [m/s]
	double w; // [m/s]
	// velocity
	double uStd; // [m/s]
	double vStd; // [m/s]
	double wStd; // [m/s]

public:
	BodyLinearRateData(string name);
	BodyLinearRateData(void);
	BodyLinearRateData(const BodyLinearRateData& obj) { *this = obj; }
    ~BodyLinearRateData(void) {};

    BodyLinearRateData & operator = (const BodyLinearRateData &obj);
    void assign(DvlData dvlData);
    void assign(AhrsData ahrsData);
    void assign(double sigmaU,double sigmaV,double sigmaW) {uStd=sigmaU;vStd=sigmaV;wStd=sigmaW;}

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

#endif /* BODYLINEARRATEDATA_H_ */
