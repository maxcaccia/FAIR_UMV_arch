/*
 * BlueBoatLPRIthread.h
 *
 *  Created on: Jul 28, 2025
 *      Author: mc
 *
  */

#ifndef BLUEBOATLPRITHREAD_H_
#define BLUEBOATLPRITHREAD_H_

#include "LPRIthread.h"
#include "CntrlSimThread.h"
//#include "ForceMoment.h"
#include "BlueBoatTCA.h"
#include "NormalisedRpm.h"
//#include "TCAstate.h"

class BlueBoatLPRIthread: public LPRIthread {
public:
	BlueBoatLPRIthread();
	virtual ~BlueBoatLPRIthread();

	void make_thread(void);

	bool init_configuration(void);

protected:
//    void process_set_left_right_rpm(void);
//    void process_set_manual_mode(void);
//    void process_arm(void);
//    void process_disarm(void);
//    void process_map_servo_default(void);
//    void process_map_servo_rc(void);

//    void process_data(char *data,ssize_t nRecv);

    //    void process_data(char *data,ssize_t nRecv) { printf("TCAthread::process_data() %s %ld\n",mName.c_str(),nRecv); } // function to be overwritten
//    void execute_thread_semantics(void); // { mTCA->execute(); /*printf("SwampTCAthread::execute_thread_semantics() %s\n",mName.c_str());*/ } // function to be overwritten

    void execute_thread_semantics(void) { /*printf("BlueBoatLPRIthread::execute_thread_semantics\n");*/LPRIthread::execute_thread_semantics(); ((BlueBoatTCA*)mTCA)->execute(); mP2LSM->execute(); }

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
    NormalisedRpm mBlueBoatRpmRef; // reference robot actuation
    NormalisedRpm mBlueBoatRpmMeas; // measured robot actuation
    // <Robot>Actuation m<Robot>ActuationMeas; // measured robot actuation

//protected:
//    BlueBoatTcaCmd mBlueBoatTcaCmd;
};



#endif /* BLUEBOATLPRITHREAD_H_ */
