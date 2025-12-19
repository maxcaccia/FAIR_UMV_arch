/*
 * CntrlSimThread.h
 *
 *  Created on: Nov 227, 2023
 *      Author: mc
 */

#ifndef CNTRLSIMTHREAD_H_
#define CNTRLSIMTHREAD_H_

#include "GenericThread.h"

class CntrlSimThread: public GenericThread {
public:
	CntrlSimThread();
	virtual ~CntrlSimThread();

	bool build_comms(void);

	virtual void make_thread(void);

	void set_properties(ThreadProperties threadProperties);

protected:

virtual void execute(void) {GenericThread::execute();}
    ssize_t receive_data(char *data); // { /*printf("virtual CntrlSimThread::receive_data() %s\n",mName.c_str());*/ return mMasterUdp.recv((unsigned char*)data,PACKET_SIZE,0);} // function to be overwritten: specific for drivers
//    virtual void process_data(char *data) { printf("virtual CntrlSimThread::process_data() %s\n",mName.c_str()); } // function to be overwritten
//    virtual void execute_thread_semantics(void) { printf("virtual CntrlSimThread::execute_thread_semantics() %s\n",mName.c_str()); } // function to be overwritten
    void send_commands(void);

protected:
	// mMasterUdp: send commands and receive data from subscribed
	UdpSocket mMasterUdp;
	RingBuffer<Packet> mCmdSendBuffer[MAX_SOCK_ADDR];
};

#endif /* CNTRLSIMTHREAD_H_ */
