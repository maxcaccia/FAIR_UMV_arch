/*
 * MIPthread.h
 *
 *  Created on: December 4, 2023
 *      Author: mc
 */

//***** insert your code here ************************************
//
//	#ifndef <module>THREAD_H_
//	#define <module>THREAD_H_
//
#ifndef MIPTHREAD_H_
#define MIPTHREAD_H_
//***** end insert your code *************************************

#include "DriverThread.h"

//***** insert your code here ************************************
//
//	#include "SerialPosix.h" // if the device is connected through serial link
//	#include "<modue>dataParser.h"
//
#include "SerialPosix.h"
#include "MIPdataParser.h"
//***** end insert your code *************************************

//***** insert your code here ************************************
//
//	class <module>thread: public DriverThread {
//	public:
//		<module>thread(void);
//		virtual ~<module>thread();
//
//***** end insert your code *************************************
class MIPthread: public DriverThread {
public:
	MIPthread(void);
	virtual ~MIPthread();

	void make_thread(void);

	void end(void);

protected:
	void execute(void) {DriverThread::execute();}
	//
	// function executing thread semantics
	//	input:	mDataParser --> *m<DataType>data
	//	output:	mDataSendBuffer
	//
	void execute_thread_semantics(void);

    void build_commands(void) {}
    void build_data(void);
	//
	// function instantiating the correct data parser mDataParser as MIPdataParser
	//
	//***** insert your code here ************************************
	//
	//	void instantiate_data_parser(void) { mDataParser = new <module>dataParser(); }
	//
	//***** end insert your code *************************************
	void instantiate_data_parser(void) { mDataParser = new MIPdataParser(); }

protected:
	//
	// type of data managed
	//
	//***** insert your code here ************************************
	//
	//	<DATATYPE>data *m<DataType>Data
	//
	//***** end insert your code *************************************
	AhrsData *mAhrsData;
};

#endif /* DAGPSTHREAD_H_ */
