/*
 * MinionUDPthread.h
 *
 *  Created on: February 5, 2024
 *      Author: mc
 */

#ifndef MINIONUDPTHREAD_H_
#define MINIONUDPTHREAD_H_

#include "DriverThread.h"

//***** insert your code here ************************************
//
//	#include "SerialPosix.h" // if the device is connected through serial link
//	#include "<module>dataParser.h"
//
//***** end insert your code *************************************
#include "MinionDataParser.h"
#include "Commands.h"

//***** insert your code here ************************************
//
//	class <module>thread: public DriverThread {
//	public:
//		<module>thread(void);
//		virtual ~<module>thread();
//
class MinionUDPthread: public DriverThread {
public:
	MinionUDPthread(void);
	virtual ~MinionUDPthread();
//***** end insert your code *************************************

	void make_thread(void);

	void end(void);

protected:

	//
	// function executing thread semantics
	//	input:	mDataParser --> *m<DataType>data
	//	output:	mDataSendBuffer
	//
	//	input:  process_command(.) --> mMinionCmd
	//  output: mCmdSendBuffer
	//
	void execute_thread_semantics(void);

    void build_commands(void) {}
    void build_data(void);
	//
	// function instantiating the correct data parser mDataParser as MinionDataParser
	//
	void instantiate_data_parser(void) { mDataParser = new MinionDataParser(); }

	void process_command(char *cmd,ssize_t n);

protected:
	//
	// type of data managed
	//
	//***** insert your code here ************************************
	//
	//	<DATATYPE>data *m<DataType>Data
	//
	PositionMotorData *mPositionMotorData;
	SpeedMotorData *mSpeedMotorData;
	AhrsData *mAhrsData;
	GnssPositionData *mGnssPosData;
	BatteryData *mBatteryData;
	//***** end insert your code *************************************

	PositionMotorCmd mPositionMotorCmd;
	SpeedMotorCmd mSpeedMotorCmd;
	GenericCmd mGenericCmd;

private:
	void set_data_names(void);
	// internal variables
	double mAzimuthRefUnwrap;
	int mNopCounter;
};

#endif /* MINIONUDPTHREAD_H_ */
