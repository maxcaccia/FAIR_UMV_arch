/*
 * LoggerBlueBoatThread.h
 *
 *  Created on: May 9, 2025
 *      Author: mc
 */

#ifndef LOGGERBLUEBOATTHREAD_H_
#define LOGGERBLUEBOATTHREAD_H_

#include "CntrlSimThread.h"

#include "NormalisedRpm.h"
#include "ForceMoment.h"

#include "AhrsData.h"
#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "BatteryData.h"

class LoggerBlueBoatThread: public CntrlSimThread {
public:
	LoggerBlueBoatThread();
	virtual ~LoggerBlueBoatThread();

	void make_thread(void);

protected:
 void execute(void) {CntrlSimThread::execute();}
    void process_command(char *cmd,ssize_t n); // function to be overwritten
    void process_data(char *data,ssize_t nRecv); // function to be overwritten
    void execute_thread_semantics(void); // function to be overwritten

private:
    void log_data(void);
    void log_headers(void);

private:
    NormalisedRpm mNormRpm;
    ForceMoment mForceMoment;

	AhrsData mAhrsData;
	GnssPositionData mGnssPosData;
	GnssVelocityData mGnssVelData;
	BatteryData mBatteryData;


    FILE *mLogFile;
};



#endif /* LOGGERBLUEBOATTHREAD_H_ */
