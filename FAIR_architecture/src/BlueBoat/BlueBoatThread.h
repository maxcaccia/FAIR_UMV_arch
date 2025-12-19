/*
 * BlueBoatThread.h
 *
 *  Created on: March 23, 2025
 *      Author: mc
 */

#ifndef BLUEBOATTHREAD_H_
#define BLUEBOATTHREAD_H_

#include "DriverThread.h"

#define THROTTLE_LEFT	73
#define THROTTLE_RIGHT	74

//***** insert your code here ************************************
//
//	#include "SerialPosix.h" // if the device is connected through serial link
//	#include "<module>dataParser.h"
//
//***** end insert your code *************************************
#include "BlueBoatDataParser.h"
#include "Commands.h"
#include "ForceMoment.h"

//***** insert your code here ************************************
//
//	class <module>thread: public DriverThread {
//	public:
//		<module>thread(void);
//		virtual ~<module>thread();
//
class BlueBoatThread: public DriverThread {
public:
	BlueBoatThread(void);
	virtual ~BlueBoatThread();
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
	void instantiate_data_parser(void) { mDataParser = new BlueBoatDataParser(); }

	void process_command(char *cmd,ssize_t n);

protected:
	//
	// type of data managed
	//
	//***** insert your code here ************************************
	//
	//	<DATATYPE>data *m<DataType>Data
	//
	bool mHeartBeatFlag;
	NormalisedRpm *mNormRpm;
	AhrsData *mAhrsData;
	GnssPositionData *mGnssPosData;
	GnssVelocityData *mGnssVelData;
	BatteryData *mBatteryData;
	//***** end insert your code *************************************

	BlueBoatCmd mBlueBoatCmd;
//	ForceMoment *mForceTorqueRef;
	NormalisedRpm *mNormRpmRef;

private:
	void set_data_names(void);
	// internal variables
	int mNopCounter;
};

#endif /* BLUEBOATTHREAD_H_ */
