/*
 * GNSSthread.h
 *
 *  Created on: February 13, 2024
 *      Author: mc
 */

#ifndef GNSSTHREAD_H_
#define GNSSTHREAD_H_

#include "DriverThread.h"

//***** insert your code here ************************************
//
//	#include "SerialPosix.h" // if the device is connected through serial link
//	#include "<module>dataParser.h"
//
//***** end insert your code *************************************
//#include "SerialPosix.h"
#include "NMEAdataParser.h"

class GNSSthread: public DriverThread {
public:
	GNSSthread(void);
	virtual ~GNSSthread();
//***** end insert your code *************************************

	void make_thread(void);

	void end(void);

protected:
	//
	// function executing thread semantics
	//	input:	mDataParser --> *m<DataType>data
	//	output:	mDataSendBuffer
	//
	void execute_thread_semantics(void);

    void build_commands(void) {}
    void build_data(void);
	//
	// function instantiating the correct data parser mDataParser as NMEAdataParser
	//
	void instantiate_data_parser(void) { mDataParser = new NMEAdataParser(); }

//	bool init_configuration(void) { mGnssPositionData->set_name(mName); mGnssVelocityData->set_name(mName); printf("GNSSthread %s\n",mGnssPositionData->mName.c_str()); return true;}

protected:
	//
	// type of data managed
	//	<DATATYPE>data *m<DataType>Data
	//
	GnssPositionData *mGnssPositionData;
	GnssVelocityData *mGnssVelocityData;
};

#endif /* GNSSTHREAD_H_ */
