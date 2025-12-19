/*
 * XYNnorm.h
 *
 *  Created on: 21 July 2025
 *      Author: massimo
 */
#ifndef XYNNORM_H_
#define XYNNORM_H_

#include "TimeStampedData.h"

#define XYN_NORM_CODE "XYN_NORM"

class XYNnorm : public TimeStampedData {
public:
	// applied force to the vehicle rigid body
	double X; // [%]
	double Y; // [%]
	// applied moment to the vehicle rigid body
	double N; // [%]

public:
	XYNnorm(string name);
	XYNnorm(void);
	XYNnorm(const XYNnorm& obj) { *this = obj; }
    ~XYNnorm(void) {};

    XYNnorm & operator = (const XYNnorm &obj);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

//private:
	void reset(void);
};

#endif /* XYNNORM_H_ */
