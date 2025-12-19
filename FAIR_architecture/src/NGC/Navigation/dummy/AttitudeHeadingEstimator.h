/*
 * AttitudeHeadingEstimator.h
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#ifndef ATTITUDEHEADINGESTIMATOR_H_
#define ATTITUDEHEADINGESTIMATOR_H_

#include <stdio.h>

#include <inttypes.h>

#include <string>
using namespace std;

#include "LogicalAttitudeData.h"
#include "LogicalAngularRateData.h"

class AttitudeHeadingEstimator
{
private:
	double *mTimeStamp;
	string mName;
	// input: measurements
	LogicalAttitudeData *mAttitudeData;
	LogicalAngularRateData *mAngularRateData;
	// output: estimates
	LogicalAttitudeData *mAttitudeHat;
	LogicalAngularRateData *mAngularRateHat;

public:
	// task constructor
	AttitudeHeadingEstimator(void);
	~AttitudeHeadingEstimator(void) {};

	void set_name(string name) { mName=name; }
	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }

	void set_input_output(LogicalAttitudeData *attitudeData,LogicalAngularRateData *angularRateData,
			LogicalAttitudeData *attitudeHat,LogicalAngularRateData *angularRateHat);

	void execute(void);
	void reset(void);

	void print(void);
};


#endif /* ATTITUDEHEADINGESTIMATOR_H_ */
