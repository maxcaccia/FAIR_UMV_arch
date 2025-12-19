/*
 * DAGPSthread.h
 *
 *  Created on: December 5, 2023
 *      Author: mc
 */

//***** insert your code here ************************************
//
//	#ifndef <module>THREAD_H_
//	#define <module>THREAD_H_
//
//***** end insert your code *************************************
#ifndef DAGPSTHREAD_H_
#define DAGPSTHREAD_H_

#include "DriverThread.h"

//***** insert your code here ************************************
//
//	#include "SerialPosix.h" // if the device is connected through serial link
//	#include "<modue>dataParser.h"
//
//***** end insert your code *************************************
#include "SerialPosix.h"
#include "NMEAdataParser.h"

//***** insert your code here ************************************
//
//	class <module>thread: public DriverThread {
//	public:
//		<module>thread(void);
//		virtual ~<module>thread();
//
class DAGPSthread: public DriverThread {
public:
	DAGPSthread(void);
	virtual ~DAGPSthread();
//***** end insert your code *************************************

	void make_thread(void);

	void end(void);

protected:
//    void execute(void) {printf("%s DAGPSthread::execute\n",mName.c_str());fflush(stdout);DriverThread::execute();}
	//
	// function executing thread semantics
	//	input:	mDataParser --> *m<DataType>data
	//	output:	mDataSendBuffer
	//
	void execute_thread_semantics(void);

	//
	// function instantiating the correct data parser mDataParser as MIPdataParser
	//
	//***** insert your code here ************************************
	//
	//	void instantiate_data_parser(void) { mDataParser = new <module>dataParser(); }
	//
	void instantiate_data_parser(void) { mDataParser = new NMEAdataParser(); }
	//***** end insert your code *************************************

protected:
	//
	// type of data managed
	//
	//***** insert your code here ************************************
	//
	//	<DATATYPE>data *m<DataType>Data
	//
	GnssPositionData *mGnssPositionData;
	GnssVelocityData *mGnssVelocityData;
	TrueHeadingData *mTrueHeadingData;
	//***** end insert your code *************************************
};

#endif /* DAGPSTHREAD_H_ */
