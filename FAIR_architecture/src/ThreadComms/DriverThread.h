/*
 * DriverThread.h
 *
 *  Created on: Nov 2, 2023
 *      Author: mc
 */

#ifndef DRIVERTHREAD_H_
#define DRIVERTHREAD_H_

#include "GenericThread.h"
#include "SerialPosix.h"
#include "DataParser.h"

class DriverThread: public GenericThread {
public:
	DriverThread(void);
	virtual ~DriverThread();

	bool build_comms(void);

	virtual void make_thread(void);

	virtual void end(void);

	void set_is_actual(bool flag) { /*printf("DriverThread::set_is_actual %s %d\n",mName.c_str(),(int)flag);*/ mIsActual=flag; }
protected:
virtual void execute(void) {GenericThread::execute();}

    ssize_t receive_data(char *data); // function to be overwritten: specific for drivers
    void process_data(char *data,ssize_t nRecv); // function to be overwritten
    virtual void execute_thread_semantics(void) { printf("virtual DriverThread::execute_thread_semantics() %s\n",mName.c_str()); } // function to be overwritten
    void send_commands(void);

	virtual void instantiate_data_parser(void) { printf("DriverThread::instantiate_data_parser %s\n",mName.c_str()); mDataParser = new DataParser(); }

protected:
	UdpSocket mVirtualDeviceUdp; // used for virtual device
	UdpSocket mActualDeviceUdp;
	SerialPosix *mActualDeviceSerial;

	RingBuffer<Packet> mCmdSendBuffer;

	DataParser *mDataParser;

	bool mIsActual;

	struct sockaddr_in mAddr;

	bool actualDevicePeerAdded;
};

#endif /* DRIVERTHREAD_H_ */
