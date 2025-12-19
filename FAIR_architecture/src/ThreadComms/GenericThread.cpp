/*
 * GenericThread.cpp
 *
 *  Created on: Oct 27, 2023
 *      Author: mc
 */

#include "GenericThread.h"

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

GenericThread::GenericThread(void): Thread() {

//	printf("GenericThread::GenericThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);

	mElapsedMilliseconds=0;
	mOldTimeStamp=0;

	mTimeStamp=0.;
}

GenericThread::~GenericThread() {
	// TODO Auto-generated destructor stub
}

void GenericThread::set_properties(ThreadProperties threadProperties) {
	mThreadProperties=threadProperties;
	Thread::set_properties(threadProperties.name, threadProperties.policy, threadProperties.priority);
	string str;
	str=mName+".mDataSendBuffer";
	mDataSendBuffer.set_name(str);
	printf("%s --- GenericThread::set_properties policy %d priority %d\n",mName.c_str(),mPolicy,mPriority);
}

bool GenericThread::build_comms(void) {
	//
	// open & bind slave UDP
	//
	if(!mSlaveUdp.bind_udp(mThreadProperties.ipAddress, mThreadProperties.cmdRecvUdpPort)) {
		printf("GenericThread::build_comms ERROR - slave UDP bind failed: %s %d\n",
				mThreadProperties.ipAddress.c_str(), mThreadProperties.cmdRecvUdpPort);
		return false;
	}
	printf("%s --- GenericThread::build_comms mSlaveUdp binded at ipAddress %s port %d\n",
			mName.c_str(),mThreadProperties.ipAddress.c_str(),mThreadProperties.cmdRecvUdpPort);

//	if(!mMasterUdp.bind_udp(mThreadProperties.ipAddress, mThreadProperties.dataRecvUdpPort)) {
//		printf("GenericThread::build_comms ERROR - master UDP bind failed: %s %d\n",
//				mThreadProperties.ipAddress.c_str(), mThreadProperties.dataRecvUdpPort);
//		return false;
//	}
//	//
//	// add "command_to" peers
//	//
//	for(int i=0;i<mThreadProperties.nCommandTo;i++) {
//		char address[64];
//		strcpy(address,mThreadProperties.commandToIpAddress[i].c_str());
//		if(!mMasterUdp.add_peer(address, (uint8_t)mThreadProperties.commandToPort[i])) {
//			printf("GenericThread::build_comms ERROR - master command_to add peer failed: %s %d\n",
//					mThreadProperties.commandToIpAddress[i].c_str(), mThreadProperties.commandToPort[i]);
//			return false;
//		}
//	}

	return true;
}

bool GenericThread::add_subscription(string ipAddress,uint16_t port) {
	char address[64];
	strcpy(address,ipAddress.c_str());
	if(!mSlaveUdp.add_peer(address,port)) {
		printf("GenericThread::add_subscription ERROR - slave add peer failed: %s %d\n",
				ipAddress.c_str(), port);
		return false;
	}
	return true;
}

bool GenericThread::remove_subscription(string ipAddress,uint16_t port) {
	char address[64];
	strcpy(address,ipAddress.c_str());
	if(!mSlaveUdp.remove_peer(address,port)) {
		printf("GenericThread::remove_subscription ERROR - slave remove peer failed: %s %d\n",
				ipAddress.c_str(), port);
		return false;
	}
	return true;
}


void GenericThread::make_thread()
{
    printf("GenericThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&GenericThread::execute,this);
}

void GenericThread::execute(void) {
    init();
	if(mThreadProperties.moduleType=="SYNCHRONOUS")
		execute_synchronous();
	else
		execute_asynchronous();
}

void GenericThread::execute_synchronous(void) {
    printf("%s --- GenericThread::execute_synchronous with period equal to %d milliseconds ...\n\n",mName.c_str(),mThreadProperties.timeValue);
    mOldTimeStamp=timeSinceEpochMillisec();
    mNextTimePoint=chrono::steady_clock::now();
    //
    // execute synchronous loop
    //
    mNextTimePoint=mNextTimePoint+chrono::milliseconds(mThreadProperties.timeValue);
    while(!mEnd)
       {
           std::this_thread::sleep_until(mNextTimePoint);
           mNextTimePoint=mNextTimePoint+chrono::milliseconds(mThreadProperties.timeValue);
           execute_body();
       }

}
void GenericThread::execute_asynchronous(void) {
    printf("%s --- GenericThread::execute_asynchronous with time out equal to %d seconds...\n\n",mName.c_str(),mThreadProperties.timeValue);
    // set mSlaveUdp as blocking (cmd receive)
    struct timeval tm;
    tm = { mThreadProperties.timeValue , 0 };
    mSlaveUdp.enable_blocking(&tm);
    //
    // execute asynchronous loop
    //
    while(!mEnd)
    	execute_body();
}


void GenericThread::execute_body(void) {
//	printf("\nGenericThread::execute_body %s %" PRIu64 "\n\n",mName.c_str(),timeSinceEpochMillisec());
	//
	// check time scheduling for synchronous threads
	//
	if(mThreadProperties.moduleType=="SYNCHRONOUS")
		check_synchronous();

	compute_time_stamp();
	//
	// receive and process commands
	// 		if ASYNCHRONOUS thread the recv inside receive_commands is blocking
	//		recv returns -1 after time out
	//	Note:	it is possible to use recvfrom to handle different behaviours according to the cmd sender,
	//			this has to be implemented
	//
	char cmd[PACKET_SIZE];
	ssize_t n;
	while((n=receive_command(cmd))>0)
		process_command(cmd,n);
	//
	// receive and process data
	//
	char data[PACKET_SIZE];
	ssize_t nRecv;
	while((nRecv=receive_data(data))>0)
	{
		process_data(data,nRecv);
	}
	//
	// execute thread semantics
	//
	execute_thread_semantics();
	//
	// build commands and append them to mCmdSendBuffer
	//
	build_commands();
	//
	// build data packets and append them to mDataSendBuffer
	//
	build_data();
	//
	// send commands to suitable "command to" peers
	//
	send_commands();
	//
	// send data to all the subscribers
	//
	send_data();
}

void GenericThread::send_data(void) {
//	printf("%s --- GenericThread::send_data() %s\n",mName.c_str(),mDataSendBuffer.get_name().c_str());

	Packet item;
	while(mDataSendBuffer.pop(item)) {
		mSlaveUdp.send((const unsigned char*)item.data, item.n, 0);
//		printf("%s --- GenericThread::send_data() %s\n",mName.c_str(),(char*)item.data);fflush(stdout);
//		printf("%s --- GenericThread::send_data() %d\n",mName.c_str(),item.n);fflush(stdout);
	}
}


void GenericThread::check_synchronous(void) {
	//printf("%s --- GenericThread::check_synchronous()\n",mName.c_str());fflush(stdout);
	uint64_t timeStamp;
	timeStamp=timeSinceEpochMillisec();
	int64_t tmp;
	tmp=timeStamp-mOldTimeStamp-mThreadProperties.timeValue;
	if(abs(tmp)>1) {
		printf("================================ virtual GenericThread::check_synchronous problems in respecting time scheduling %" PRIu64 " %" PRIu64 " %" PRId64 "\n",timeStamp,mOldTimeStamp+mThreadProperties.timeValue,tmp);
		fflush(stdout);
	}
	mOldTimeStamp=timeStamp;
	mElapsedMilliseconds+=mThreadProperties.timeValue;
//	printf("%s --- GenericThread::check_synchronous() %" PRIu64 "\n",mName.c_str(),timeStamp);fflush(stdout);

}


void GenericThread::compute_time_stamp(void) {
	//
	// set time stamp & valid flag
	//
    time_t t;
    struct tm *tmp;
    t = time(NULL);
    tmp = gmtime(&t);
    mTimeStamp = tmp->tm_hour*10000.+tmp->tm_min*100.+tmp->tm_sec+(mElapsedMilliseconds%1000)/1000.; // hhmmss.ss in UTC

}
