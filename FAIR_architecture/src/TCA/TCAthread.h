/*
 * TCAthread.h
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 *
 *  TCAthread is the parent class for <Robot>TCAthread
 */

#ifndef TCATHREAD_H_
#define TCATHREAD_H_

#include "CntrlSimThread.h"
#include "ForceMoment.h"
#include "TCAstate.h"
#include "TCA.h"
#include "Commands.h"

class TCAthread: public CntrlSimThread {
public:
	TCAthread();
	virtual ~TCAthread();

	virtual void make_thread(void);

	virtual bool init_configuration(void);

protected:
    virtual void process_command(char *cmd,ssize_t n); // { printf("TCAthread::process_command() %s\n",mName.c_str()); } // function to be written
    void process_set_n_dof(void) { mForceMomentRef.X=mTcaCmd.X;mForceMomentRef.Y=mTcaCmd.Y;mForceMomentRef.Z=mTcaCmd.Z;mForceMomentRef.K=mTcaCmd.K;mForceMomentRef.M=mTcaCmd.M;mForceMomentRef.N=mTcaCmd.N; }
    void process_set_mode(void) { mTCA->set_mode(mTcaCmd.mode); }
    void process_set_function(void) { mTCA->set_function(mTcaCmd.function); }
    void process_set_config(void) { mTCA->set_config(mTcaCmd.config); }
	//    virtual void process_data(char *data,ssize_t nRecv) { printf("TCAthread::process_data() %s %ld\n",mName.c_str(),nRecv); } // function to be overwritten
    virtual void execute_thread_semantics(void) { mTCAstate.mTimeStamp = mTimeStamp;/*printf("TCAthread::execute_thread_semantics() %s\n",mName.c_str());*/ } // function to be overwritten

    virtual void build_data(void);

protected:
    ////////////
    //
    // TCA data: references
    //
    //
    // robot force and torque
    //
    ForceMoment mForceMomentRef; // reference force and moment
    ForceMoment mForceMomentHat; // estimated force and moment
    //
    // robot actuation : to be declared in the child class <Robot>TCAthread
    //
    // <Robot>Actuation m<Robot>ActRef; // reference robot actuation
    // <Robot>Actuation m<Robot>ActMeas; // measured robot actuation
    //
    // state
    TCAstate mTCAstate;
    ////////////
    //
    // TCA modules
    //
    TCA* mTCA;

protected:
    TcaCmd mTcaCmd;

};



#endif /* TCATHREAD_H_ */
