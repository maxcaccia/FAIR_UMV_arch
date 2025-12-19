/*
 * DriverThread.cpp
 *
 *  Created on: Nov 1, 2023
 *      Author: mc
 */

#include "DriverThread.h"

#include <arpa/inet.h>

DriverThread::DriverThread(void): GenericThread() {
//	printf("DriverThread::DriverThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mActualDeviceSerial=NULL;
	mIsActual=false;
	mDataParser=NULL;
	actualDevicePeerAdded=false;
}

DriverThread::~DriverThread() {
	// TODO Auto-generated destructor stub
}

bool DriverThread::build_comms(void) {
	//
	// open & bind slave UDP
	//
	GenericThread::build_comms();
	//
	// create comms to/from device
	if(!mIsActual) {
		//
		// open & bind virtual UDP
		//
		if(!mVirtualDeviceUdp.bind_udp(mThreadProperties.ipAddress, mThreadProperties.dataRecvUdpPort)) {
			printf("DriverThread::build_comms ERROR - virtual device UDP bind failed: %s %d\n",
					mThreadProperties.ipAddress.c_str(), mThreadProperties.dataRecvUdpPort);
			return false;
		}
		printf("%s --- DriverThread::build_comms mVirtualDeviceUdp binded at ipAddress %s port %d\n",
			mName.c_str(),mThreadProperties.ipAddress.c_str(),mThreadProperties.dataRecvUdpPort);
		string str;
		str=mName+".mCmdSendBuffer";
		mCmdSendBuffer.set_name(str);
		//
		// add virtual device peer
		//
		char address[64];
		strcpy(address,mThreadProperties.virtualDeviceIpAddress.c_str());
		if(!mVirtualDeviceUdp.add_peer(address, (uint16_t)mThreadProperties.virtualDevicePort)) {
			printf("DriverThread::build_comms ERROR - virtual device add peer failed: %s %d\n",
					mThreadProperties.virtualDeviceIpAddress.c_str(), mThreadProperties.virtualDevicePort);
			return false;
		}
		printf("%s --- DriverThread::build_comms mVirtualDeviceUdp added peer at ipAddress %s port %d\n",
			mName.c_str(),address,mThreadProperties.virtualDevicePort);

	}
	else { // actual device
		if(mThreadProperties.actualDeviceType=="UDP_SOCKET") {
			//
			// open & bind actual UDP
			//
			if(!mActualDeviceUdp.bind_udp(mThreadProperties.ipAddress, mThreadProperties.dataRecvUdpPort)) {
				printf("DriverThread::build_comms ERROR - actual device UDP bind failed: %s %d\n",
						mThreadProperties.ipAddress.c_str(), mThreadProperties.dataRecvUdpPort);
				return false;
			}
			printf("%s --- DriverThread::build_comms mActualDeviceUdp binded at ipAddress %s port %d\n",
					mName.c_str(),mThreadProperties.ipAddress.c_str(),mThreadProperties.dataRecvUdpPort);
			//
			// add actual device peer
			//
			string stringa=mThreadProperties.actualDeviceIpAddress.c_str();
			if(stringa!="0.0.0.0") {
				char address[64];
				strcpy(address,mThreadProperties.actualDeviceIpAddress.c_str());
				if(!mActualDeviceUdp.add_peer(address, (uint16_t)mThreadProperties.actualDevicePort)) {
					printf("DriverThread::build_comms ERROR - actual device add peer failed: %s %d\n",
							mThreadProperties.actualDeviceIpAddress.c_str(), mThreadProperties.actualDevicePort);
					return false;
				}
				string str;
				str=mName+".mCmdSendBuffer";
				mCmdSendBuffer.set_name(str);

				actualDevicePeerAdded=true;

				printf("DriverThread::build_comms actualDevicePeerAdded: %s %d\n",
					mThreadProperties.actualDeviceIpAddress.c_str(), mThreadProperties.actualDevicePort);
			}
		}
		else if(mThreadProperties.actualDeviceType=="RS-232") {
			//
			// instantiate actual RS-232
			//
			struct SerialPosixPar serialPosixPar;
			serialPosixPar.ttyName=mThreadProperties.actualDeviceTty;
			serialPosixPar.blocking=mThreadProperties.rs232blocking;
			serialPosixPar.canonicalMode=mThreadProperties.rs232canonicalMode;
			serialPosixPar.baudRate=mThreadProperties.rs232baudRate;
			serialPosixPar.parityEnable=mThreadProperties.rs232parityEnable;
			serialPosixPar.parityType=mThreadProperties.rs232parityType;
			serialPosixPar.stopBits=mThreadProperties.rs232stopBits;
			serialPosixPar.csNumber=mThreadProperties.rs232csNumber;
			string rs232name=mName+"_rs232";
			mActualDeviceSerial = new SerialPosix(rs232name,&serialPosixPar);
			//
			// open/create actual RS-232
			//

//			printf("DriverThread::build_comms - create SerialPosix %s ----- uncomment code for creating SerialPosix\n",rs232name.c_str());

			if(!mActualDeviceSerial->create()) {
				printf("DriverThread::build_comms ERROR - actualDeviceType %s not supported\n",rs232name.c_str());
				return false;
			}

//			printf("%s --- DriverThread::build_comms mActualDeviceSerial created on %s at baud rate %" PRIu32 "\n",
//					mName.c_str(),mThreadProperties.actualDeviceTty.c_str(),mThreadProperties.rs232baudRate);

		}
		else {
			printf("DriverThread::build_comms ERROR - RS-232 create failed: %s\n",mThreadProperties.actualDeviceType.c_str());
			return false;
		}
	}

	//
	// instantiate data parser
	//
	instantiate_data_parser();

	return true;
}


void DriverThread::make_thread()
{
    printf("DriverThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&DriverThread::execute,this);
}

void DriverThread::end(void)
{
	printf("DriverThread::end %s\n",mName.c_str());
	if(mActualDeviceSerial!=NULL) {
		mActualDeviceSerial->end();
		delete mActualDeviceSerial;
		mActualDeviceSerial=NULL;
	}
	if(mDataParser!=NULL) {
		delete mDataParser;
		mDataParser=NULL;
	}
}


ssize_t DriverThread::receive_data(char *data) {
	//printf("%s --- DriverThread::receive_data isActual %d\n",mName.c_str(),mIsActual);

	if(!mIsActual) { // virtual device : UdpSocket mVirtualDeviceUdp
		return mVirtualDeviceUdp.recv((unsigned char*)data, PACKET_SIZE, 0);
	}
	else { // actual device
		if(mThreadProperties.actualDeviceType=="UDP_SOCKET") { // actual device UDP : UdpSocket mActualDeviceUdp
//			return mActualDeviceUdp.recv((unsigned char*)data, PACKET_SIZE, 0);
//			return mActualDeviceUdp.recv((unsigned char*)data, PACKET_SIZE, 0,mAddr);

			ssize_t n;
			n=mActualDeviceUdp.recv((unsigned char*)data, PACKET_SIZE, 0,mAddr);

			char ip[INET_ADDRSTRLEN];
			struct sockaddr_in *sin = (struct sockaddr_in *)&mAddr;
			uint16_t port;
			inet_ntop (AF_INET, (const void*)&(sin->sin_addr), ip, sizeof (ip));
			port = htons (sin->sin_port);
//			printf ("host %s:%d\n", ip, port);

			if((n>0)&&(!actualDevicePeerAdded)) {
				//
				// add actual device peer
				//
				char address[64];
				strcpy(address,ip);
				if(!mActualDeviceUdp.add_peer(address, port)) {
					printf("DriverThread::receive_data ERROR - actual device add peer failed: %s %d\n",
							address, port);
					return false;
				}
				string str;
				str=mName+".mCmdSendBuffer";
				mCmdSendBuffer.set_name(str);

				actualDevicePeerAdded=true;

				printf("DriverThread::build_comms actualDevicePeerAdded: %s %d\n",
					address, port);

			}



			return n;



		}
		else if(mThreadProperties.actualDeviceType=="RS-232") { // actual device RS-232 : SerialPosix *mActualDeviceSerial
			if(mActualDeviceSerial!=NULL)
				return mActualDeviceSerial->get_data(data, PACKET_SIZE);
			else
				return 0;
		}
		else {
			printf("DriverThread::receive_data ERROR : %s\n",mThreadProperties.actualDeviceType.c_str());
			return 0;
		}
	}
}


void DriverThread::process_data(char *data,ssize_t nRecv) {
//	printf("DriverThread::process_data() %s\n",mName.c_str());

	if(mThreadProperties.actualDeviceType=="UDP_SOCKET")
		mDataParser->execute((unsigned char*)data, nRecv);
	else {
		for(int i=0;i<nRecv;i++)
			mDataParser->execute((uint8_t)data[i]);
	}
}


void DriverThread::send_commands(void) {
//	printf("%s --- DriverThread::send_commands isActual %d\n",mName.c_str(),(int)mIsActual);

//	string item;
	Packet item;
//	char packet[PACKET_SIZE];
	if(!mIsActual) { // virtual device : UdpSocket mVirtualDeviceUdp
		while(mCmdSendBuffer.pop(item)) {
//			int n=sprintf(packet,"%s\r\n",item.c_str());
//printf("DriverThread::send_commands %s",(char*)item.data);fflush(stdout);
			//mVirtualDeviceUdp.send((const unsigned char*)packet, n, 0);
//			mVirtualDeviceUdp.send((const unsigned char*)packet, PACKET_SIZE, 0);
			mVirtualDeviceUdp.send((const unsigned char*)item.data, item.n, 0);
		}
	}
	else { // actual device
		if(mThreadProperties.actualDeviceType=="UDP_SOCKET") { // actual device UDP : UdpSocket mActualDeviceUdp
			while(mCmdSendBuffer.pop(item)) {
////				int n=sprintf(packet,"%s\r\n",item.c_str());
////				mActualDeviceUdp.send((const unsigned char*)packet, n, 0);
				if(actualDevicePeerAdded)
					mActualDeviceUdp.send((const unsigned char*)item.data, item.n, 0);
//				mActualDeviceUdp.sendto((const unsigned char*)item.data, item.n, 0,mAddr);
			}
		}
		else if(mThreadProperties.actualDeviceType=="RS-232") { // actual device RS-232 : SerialPosix *mActualDeviceSerial
			if(mActualDeviceSerial!=NULL) {
				while(mCmdSendBuffer.pop(item)) {
//					int n=sprintf(packet,"%s\r\n",item.c_str());
					printf("DriverThread::send_commands %s mActualDeviceSerial->send to be implemented!\n",mName.c_str());
//					mActualDeviceSerial->send((const unsigned char*)packet, n, 0);
				}
			}
		}
		else {
			printf("DriverThread::send_commands ERROR : %s\n",mThreadProperties.actualDeviceType.c_str());
		}
	}
}

