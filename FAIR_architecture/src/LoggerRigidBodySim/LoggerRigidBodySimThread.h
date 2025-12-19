/*
 * LoggerRigidBodySimThread.h
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#ifndef LOGGERRIGIDBODYSIMTHREAD_H_
#define LOGGERRIGIDBODYSIMTHREAD_H_

#include "CntrlSimThread.h"

#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "ForceMoment.h"

class LoggerRigidBodySimThread: public CntrlSimThread {
public:
	LoggerRigidBodySimThread();
	virtual ~LoggerRigidBodySimThread();

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
    LinearPosition mPosition;
    AngularPosition mAttitude;
    LinearVelocity mLinearSpeed;
    AngularVelocity mAngSpeed;
    ForceMoment mForceMoment;

    FILE *mLogFile;
};



#endif /* LOGGERRIGIDBODYSIMTHREAD_H_ */
