/*
 * LoggerBlueBoatLPRIthread.h
 *
 *  Created on: Aug 6, 2025
 *      Author: mc
 */

#ifndef LOGGERBLUEBOATLPRITHREAD_H_
#define LOGGERBLUEBOATLPRITHREAD_H_

#include "CntrlSimThread.h"

#include "XYN.h"
#include "XYNnorm.h"
#include "NormalisedRpm.h"
#include "BatteryData.h"
#include "TCAstate.h"

#include "LogicalPositionData.h"
#include "LogicalAttitudeData.h"
#include "LogicalSpeedData.h"
#include "LogicalLinearRateData.h"
#include "LogicalAngularRateData.h"
#include "P2LSMstate.h"


class LoggerBlueBoatLPRIthread: public CntrlSimThread {
public:
	LoggerBlueBoatLPRIthread();
	virtual ~LoggerBlueBoatLPRIthread();

	void make_thread(void);

	bool init_configuration(void);

protected:
 void execute(void) {CntrlSimThread::execute();}
    void process_command(char *cmd,ssize_t n); // function to be overwritten
    void process_data(char *data,ssize_t nRecv); // function to be overwritten
    void execute_thread_semantics(void); // function to be overwritten

private:
    void log_data(void);
    void log_headers(void);

private:
    XYN mXYNref;
    XYN mXYNhat;
    XYNnorm mXYNnormRef;
    XYNnorm mXYNnormHat;
    NormalisedRpm mBlueBoatRpmRef;
    NormalisedRpm mBlueBoatRpmMeas;
    BatteryData mBatteryData;
    TCAstate mTCAstate;

    LogicalPositionData mLogicalPosition;
    LogicalAttitudeData mLogicalAttitude;
    LogicalSpeedData mLogicalSpeed;
    LogicalLinearRateData mLogicalLinearRate;
    LogicalAngularRateData mLogicalAngularRate;
    P2LSMstate mP2LSMstate;

    FILE *mLogFile;
};



#endif /* LOGGERBLUEBOATLPRITHREAD_H_ */
