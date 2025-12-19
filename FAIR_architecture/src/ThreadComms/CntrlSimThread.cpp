/*
 * CntrlSimThread.cpp
 *
 *  Created on: Nov 2, 2023
 *      Author: mc
 */

#include "CntrlSimThread.h"

CntrlSimThread::CntrlSimThread(void): GenericThread() {

//	printf("CntrlSimThread::CntrlSimThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);

}

CntrlSimThread::~CntrlSimThread() {
	// TODO Auto-generated destructor stub
}


bool CntrlSimThread::build_comms(void) {
	//
	// open & bind slave UDP
	//
	GenericThread::build_comms();
	//
	// open & bind master UDP
	//
	if(!mMasterUdp.bind_udp(mThreadProperties.ipAddress, mThreadProperties.dataRecvUdpPort)) {
		printf("CntrlSimThread::build_comms ERROR - master UDP bind failed: %s %d\n",
				mThreadProperties.ipAddress.c_str(), mThreadProperties.dataRecvUdpPort);
		return false;
	}
	printf("%s --- CntrlSimThread::build_comms mMasterUdp binded at ipAddress %s port %d\n",
			mName.c_str(),mThreadProperties.ipAddress.c_str(),mThreadProperties.dataRecvUdpPort);

	//
	// add "command_to" peers
	//
	for(int i=0;i<mThreadProperties.nCommandTo;i++) {
		char address[64];
		strcpy(address,mThreadProperties.commandToIpAddress[i].c_str());
		if(!mMasterUdp.add_peer(address, (uint16_t)mThreadProperties.commandToPort[i])) {
			printf("GenericThread::build_comms ERROR - master command_to add peer failed: %s %d\n",
					mThreadProperties.commandToIpAddress[i].c_str(), mThreadProperties.commandToPort[i]);
			return false;
		}
	}

	return true;
}


void CntrlSimThread::make_thread()
{
//    printf("CntrlSimThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&CntrlSimThread::execute,this);
}

void CntrlSimThread::set_properties(ThreadProperties threadProperties) {
	GenericThread::set_properties(threadProperties);
	string str0,str;
	str0=mName+".mCmdSendBuffer";
	for(int i=0;i<mThreadProperties.nCommandTo;i++) {
		str=str0+"."+mThreadProperties.commandToName[i];
		mCmdSendBuffer[i].set_name(str);
	}
}

ssize_t CntrlSimThread::receive_data(char *data) {
//	printf("virtual CntrlSimThread::receive_data() %s\n",mName.c_str());
	ssize_t n=mMasterUdp.recv((unsigned char*)data,PACKET_SIZE,0);
//	printf("%s ----- virtual CntrlSimThread::receive_data() %s\n",mName.c_str(),(char *)data);fflush(stdout);
	return n;
} // function to be overwritten: specific for drivers


void CntrlSimThread::send_commands(void) {
//	printf("%s --- CntrlSimThread::send_commands()\n",mName.c_str());
//	for(int i=0;i<mThreadProperties.nCommandTo;i++)
//		printf("\t%s\n",mCmdSendBuffer[i].get_name().c_str());

	Packet item;
	for(int i=0;i<mThreadProperties.nCommandTo;i++) {
		while(mCmdSendBuffer[i].pop(item)) {
//			char packet[PACKET_SIZE];
//			int n=sprintf(packet,"%s\r\n",item.c_str());
			mMasterUdp.select_send((const unsigned char*)item.data, item.n, 0,i);
//printf("CntrlSimThread::send_commands %s",(char*)item.data);
		}
	}
}

