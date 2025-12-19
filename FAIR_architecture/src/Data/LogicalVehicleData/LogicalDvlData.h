/*
 * LogicalDvlData.h
 *
 *  Created on: 10 January 2025
 *      Author: massimo
 */
#ifndef LOGICALDVLDATA_H_
#define LOGICALDVLDATA_H_

#include "DvlData.h"

#define LOGICAL_DVL_CODE "LOGICAL_DVL"

class LogicalDvlData : public DvlData {
public:
	// velocity
	double uStd; // [m/s]
	double vStd; // [m/s]
	double wStd; // [m/s]
	// range from bottom
	double rhoStd; // [m]

public:
	LogicalDvlData(string name);
	LogicalDvlData(void);
	LogicalDvlData(const LogicalDvlData& obj) { *this = obj; }
    ~LogicalDvlData(void) {};

    LogicalDvlData & operator = (const LogicalDvlData &obj);
    void assign(DvlData dvlData);

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

#endif /* LOGICALDVLDATA_H_ */
