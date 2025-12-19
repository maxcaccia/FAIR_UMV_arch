/*
 * XYN.h
 *
 *  Created on: 21 July 2025
 *      Author: massimo
 */
#ifndef XYN_H_
#define XYN_H_

#include "TimeStampedData.h"

#define XYN_CODE "XYN"

class XYN : public TimeStampedData {
public:
	// applied force to the vehicle rigid body
	double X; // [N]
	double Y; // [N]
	// applied moment to the vehicle rigid body
	double N; // [N m]

public:
	XYN(string name);
	XYN(void);
	XYN(const XYN& obj) { *this = obj; }
    ~XYN(void) {};

    XYN & operator = (const XYN &obj);

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

#endif /* XYN_H_ */
