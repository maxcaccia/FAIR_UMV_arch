/*
 * P2LSMstate.h
 *
 *  Created on: 10 February 2025
 *      Author: massimo
 */
#ifndef P2LSMSTATE_H_
#define P2LSMSTATE_H_

#include "TimeStampedData.h"

#define P2LSM_STATE_CODE "P2LSM_STATE"

class P2LSMstate : public TimeStampedData {
public:
    //
    // active Logical Sensors
    //
    string mActiveLogicalPosition;
    string mActiveLogicalAttitude;
    string mActiveLogicalSpeed;
    string mActiveLogicalLinearRate;
    string mActiveLogicalAngularRate;

public:
    P2LSMstate(void);
    P2LSMstate(string name);
    ~P2LSMstate(void) {};

    P2LSMstate & operator = (const P2LSMstate &obj);

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

#endif /* P2LSMSTATE_H_ */
