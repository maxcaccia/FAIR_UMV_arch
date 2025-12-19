/*
 * LoggerThread.h
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#ifndef LOGGERTHREAD_H_
#define LOGGERTHREAD_H_

#include "CntrlSimThread.h"

#include "AHRSdata.h"
#include "GPSdata.h"
#include "TrueHeadingData.h"
#include "Kinematics.h"
#include "ForceTorque.h"
#include "SwampTlm.h"

class LoggerThread: public CntrlSimThread {
public:
	LoggerThread();
	virtual ~LoggerThread();

	void make_thread(void);

protected:
 void execute(void) {CntrlSimThread::execute();}
    void process_command(char *cmd); // function to be overwritten
    void process_data(char *data,ssize_t nRecv); // function to be overwritten
    void execute_thread_semantics(void); // function to be overwritten

private:
    void log_data(void);
    void log_headers(void);

private:
    AHRSdata mAhrsData;
    GPSdata mGpsData;
    TrueHeadingData mTrueHeadingData;
    Kinematics mKinHat;
    ForceTorque mForceTorqueRef;
    SwampTlm mSwampTlm;

    FILE *mLogFile;
};



#endif /* LOGGERTHREAD_H_ */
