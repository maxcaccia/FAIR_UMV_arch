/*
 * SeaCurrent.h
 *
 *  Created on: 14 February 2025
 *      Author: massimo
 */
#ifndef SEACURRENT_H_
#define SEACURRENT_H_

#include "TimeStampedData.h"

#define SEA_CURRENT_CODE "SEA_CUR"

class SeaCurrent : public TimeStampedData {
public:
	// sea current
	double xCdot; // [m/s]
	double yCdot; // [m/s]

public:
	SeaCurrent(string name);
	SeaCurrent(void);
	SeaCurrent(const SeaCurrent& obj) { *this = obj; }
    ~SeaCurrent(void) {};

    SeaCurrent & operator = (const SeaCurrent &obj);

//    bool fread(FILE *f);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

private:
	void reset(void);
};

#endif /* SEACURRENT_H_ */
