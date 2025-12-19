/*
 * SwampTCAthread.h
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 *
  */

#ifndef SWAMPTCATHREAD_H_
#define SWAMPTCATHREAD_H_

#include "TCAthread.h"
#include "CntrlSimThread.h"
#include "ForceMoment.h"
#include "SwampTCA.h"
#include "SwampActuation.h"
#include "PositionMotorData.h"
#include "SpeedMotorData.h"
//#include "TCAstate.h"

class SwampTCAthread: public TCAthread {
public:
	SwampTCAthread();
	virtual ~SwampTCAthread();

	void make_thread(void);

	bool init_configuration(void);

protected:
    void process_command(char *cmd,ssize_t n); // { printf("SwampTCAthread::process_command() %s\n",mName.c_str()); } // function to be written
    void process_set_rpm_azimuth(void) { ((SwampTCA*)mTCA)->set_rpm_azimuth(mSwampTcaCmd.n,mSwampTcaCmd.alpha); }
    void process_set_power(void);
    void process_set_enable(void);
    void process_homing(void);
    void process_set_azimuth_offset(void);
    void process_set_azimuth_home(void);

    void process_data(char *data,ssize_t nRecv);

    //    void process_data(char *data,ssize_t nRecv) { printf("TCAthread::process_data() %s %ld\n",mName.c_str(),nRecv); } // function to be overwritten
    void execute_thread_semantics(void); // { mTCA->execute(); /*printf("SwampTCAthread::execute_thread_semantics() %s\n",mName.c_str());*/ } // function to be overwritten

    void build_commands(void);
    void build_data(void);

private:
//    bool mEnableRpmAzimuth;
    ////////////
    //
    // SWAMP TCA data: references
    //
    // robot actuation : to be declared in the child class <Robot>TCAthread
    //
    SwampActuation mSwampActRef; // reference robot actuation
    SwampActuation mSwampActMeas; // measured robot actuation
    // <Robot>Actuation m<Robot>ActuationMeas; // measured robot actuation

protected:
    SwampTcaCmd mSwampTcaCmd;

    PositionMotorCmd mAzimuthCmd;
    SpeedMotorCmd mPumpCmd;
};



#endif /* SWAMPTCATHREAD_H_ */
