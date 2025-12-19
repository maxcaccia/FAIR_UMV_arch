/*
 * LoggerTCAthread.h
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#ifndef LOGGERTCATHREAD_H_
#define LOGGERTCATHREAD_H_

#include "CntrlSimThread.h"

#include "SwampActuation.h"
#include "ForceMoment.h"
#include "TCAstate.h"

class LoggerTCAthread: public CntrlSimThread {
public:
	LoggerTCAthread();
	virtual ~LoggerTCAthread();

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
    SwampActuation mSwampActuationRef;
    SwampActuation mSwampActuationMsr;
    TCAstate mTCAstate;
    ForceMoment mForceMomentRef;
    ForceMoment mForceMomentEst;

    FILE *mLogFile;
};



#endif /* LOGGERTCATHREAD_H_ */
