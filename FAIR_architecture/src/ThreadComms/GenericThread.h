/*
 * GenericThread.h
 *
 *  Created on: Oct 27, 2023
 *      Author: mc
 */

#ifndef GENERICTHREAD_H_
#define GENERICTHREAD_H_

#include "Thread.h"
#include "ThreadProperties.h"
#include "UDPsocket.h"
#include "RingBuffer.h"
#include "Packet.h"

//#define PACKET_SIZE 4096

uint64_t timeSinceEpochMillisec();

class GenericThread: public Thread {
public:
	GenericThread();
	virtual ~GenericThread();

	virtual bool build_comms(void);
	bool add_subscription(string ipAddress,uint16_t port);
	bool remove_subscription(string ipAddress,uint16_t port);

	virtual void make_thread(void);

	virtual void set_properties(ThreadProperties threadProperties);
	virtual bool init_configuration(void) {printf("GenericThread::init_configuration\n");fflush(stdout);return true;} // function to be overwritten

protected:

	void execute(void);
    void execute_synchronous(void);
    void execute_asynchronous(void);
    void execute_body(void);

    ssize_t receive_command(char *cmd) { /*printf("%s --- GenericThread::receive_command()\n",mName.c_str());*/ return mSlaveUdp.recv((unsigned char*)cmd,PACKET_SIZE,0); }
    virtual void process_command(char *cmd,ssize_t n) { printf("virtual GenericThread::process_command() %s\n",mName.c_str()); } // function to be overwritten
    virtual ssize_t receive_data(char *data) { printf("virtual GenericThread::receive_data() %s\n",mName.c_str()); return 0;} // function to be overwritten: specific for drivers
    virtual void process_data(char *data,ssize_t nRecv) { printf("virtual GenericThread::process_data() %s %" PRId32 "\n",mName.c_str(),(int32_t)nRecv); } // function to be overwritten
    virtual void execute_thread_semantics(void) { printf("virtual GenericThread::execute_thread_semantics() %s\n",mName.c_str()); } // function to be overwritten
    virtual void build_commands(void) { /*printf("virtual GenericThread::build_commands() %s\n",mName.c_str());*/ } // function to be overwritten
    virtual void build_data(void) { /*printf("virtual GenericThread::build_data() %s\n",mName.c_str()); */} // function to be overwritten
    virtual void send_commands(void) { printf("GenericThread::send_commands() %s\n",mName.c_str()); } // function to be overwritten: specific for drivers
    void send_data(void);

    void check_synchronous(void);

    void compute_time_stamp(void);

protected:
	ThreadProperties mThreadProperties;
	// mSlaveUdp: receives commands and send data to subscribers
	UdpSocket mSlaveUdp;
	RingBuffer<Packet> mDataSendBuffer;

	uint64_t mElapsedMilliseconds;
	uint64_t mOldTimeStamp;
	chrono::steady_clock::time_point mNextTimePoint;

	double mTimeStamp;
};

#endif /* GENERICTHREAD_H_ */
