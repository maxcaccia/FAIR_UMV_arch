/*
 * <module>_launcher.cpp
 *
 *  Created on: 1 December 2023
 *      Author: Massimo Caccia
 */
#include <signal.h>

#include <string>

#include "Service.h"

//***** insert your code here ************************************
//
// #include "<module>thread.h"
//
#include "MIPthread.h"
//***** end insert your code *************************************


//======================================================================================
//
// handle Cntrl^C process exit
//
bool running=true;
void ctrl_c_handler(int dummy)
{
	running = false;
}

//======================================================================================
//
// main
//	called as: main_launcher_<module> <threadName> <config path> <serviceUDPtimeOut> <true/false>
//		where
//			<serviceUDPtimeOut> is in [s]
//			<true/false> specifies if a DRIVER is actual/virtual
//
//======================================================================================
int main(int argc, char* argv[])
{
	//======================================================================================
	//
	// check argc
	//
	if (argc < 4) {
		// <threadName>, <serviceUDPtimeOut> and true/flseare always requested
		printf("Wrong parameter number\n\tmain_launcher_<module>  <threadName> <serviceUDPtimeOut> <true/false>\n");
		fflush(stdout);
		return -1;
	}

	//======================================================================================
	//
	// check help
	//
	if (strcmp(argv[1],"help")==0) {
		printf("<module>_launcher <threadName> <config path> <serviceUDPtimeOut> <true/false>\n");
		fflush(stdout);
		return -1;
	}


	//======================================================================================
	//
	// read thread properties
	//
	// build configuration file path name
	string threadName=argv[1];
	string configDirName=CONFIG_PATH;
	string configFileName;
	configFileName=threadName+".thread";
	// read configuration file
	ThreadProperties thrProperties(configDirName);
	if(!thrProperties.fread(configFileName.c_str()))
		return 0;
	if(!thrProperties.process_dependencies())
		return 0;
	printf("============================================================================\n");fflush(stdout);
	printf("Thread Properties\n");fflush(stdout);
	printf("============================================================================\n");fflush(stdout);
	thrProperties.print();
	printf("============================================================================\n");fflush(stdout);

	//======================================================================================
	//
	// service process init
	//
	//======================================================================================
	//
	// create service UDP
	//
	UdpSocket serviceUDP;
	if(!serviceUDP.bind_udp(thrProperties.ipAddress,(uint16_t)thrProperties.serviceUdpPort))
		return 0;
	printf("\n%s serviceUDP binded at ipAddress %s port %d\n",
			thrProperties.name.c_str(),thrProperties.ipAddress.c_str(),thrProperties.serviceUdpPort);
	// set service UDP to blocking with timeout
	int serviceTimeout=stoi(argv[2]);
	struct timeval tm = { serviceTimeout , 0 };
	serviceUDP.enable_blocking(&tm);
	printf("%s serviceUDP enabled blocking with timeout %ld\n\n",thrProperties.name.c_str(),tm.tv_sec);
	//
	// build subscribe list to modules
	//
	build_subscribe_to_list(thrProperties,&serviceUDP);

	//======================================================================================
	//
	// thread
	//
	//======================================================================================
	//
	// create thread
	//
	GenericThread *genericThread=NULL;
	//***** insert your code here ************************************
	//
	//	if(thrProperties.moduleName=="<module>") {
	//		genericThread = new <module>thread();
	//	}
	//	else {
	//		printf("ERROR: <module>thread not implemented!\n"); fflush(stdout);
	//		return 0;
	//	}
	//
	if(thrProperties.moduleName=="MIP") {
		genericThread = new MIPthread();
	}
	else {
		printf("ERROR: MIPthread not implemented!\n"); fflush(stdout);
		return 0;
	}
	//***** end insert your code *************************************
	//
	// set thread properties
	//
	genericThread->set_properties(thrProperties);
	//
	// init configuration, if needed
	//
	if(!genericThread->init_configuration())
		return 0;
	//
	// check virtual/actual device in the case of DRIVER
	//		default is virtual
	//
	bool isActual=false;
	if((thrProperties.moduleClass=="DRIVER")&&(argc==4)) {
		string flag=argv[3];
		if(flag=="true")
			isActual=true;
		//***** insert your code here ************************************
		//
		// ((<module>thread*)genericThread)->set_is_actual(isActual);
		//
		((MIPthread*)genericThread)->set_is_actual(isActual);
		//***** end insert your code *************************************
	}
	//
	// build thread comms
	//
	if(!genericThread->build_comms())
		return 0;

	//
	// execute thread
	//
	genericThread->make_thread();
	printf("\n=========================================""\n=========================================\n\n");
	printf("%s in execution ...\n",thrProperties.name.c_str());
	printf("\n=========================================""\n=========================================\n\n");
	fflush(stdout);

	//======================================================================================
	//
	// service process execute
	//
	//======================================================================================
	signal(SIGINT, ctrl_c_handler);

	uint8_t packet[64];
	char peerIpAddr[64];
	uint16_t peerPort;
	while(running) {
		sleep(serviceTimeout);
		//
		// subscribe to modules
		//
		subscribe_to(thrProperties,&serviceUDP);
		//
		// receive command/acknowledgments
		//
		int n;
		while((n=serviceUDP.recvfrom(packet, 64, 0,peerIpAddr,peerPort))!=-1) {
			//
			// process command/acknowledgments
			//
			running=process_packet(packet,peerIpAddr,peerPort,&serviceUDP,genericThread,thrProperties);
		}
//		printf("=========================================== serviceUDP.recvfrom time out\n");fflush(stdout);
	}

	genericThread->end();

	printf("launcher ends...\n");

	return 0;
}


//void build_subscribe_to_list(ThreadProperties thrProperties,UdpSocket* serviceUdp) {
//	for(int i=0;i<thrProperties.nSubscribeTo;i++)
//	{
//		char address[64];
//		strcpy(address,thrProperties.subscribeToIpAddress[i].c_str());
//		serviceUdp->add_peer(address, thrProperties.subscribeToPort[i]);
//	}
//}

//void subscribe_to(ThreadProperties thrProperties,UdpSocket* serviceUdp) {
//	for(int i=0;i<thrProperties.nSubscribeTo;i++) {
//		uint8_t packet[64];
//		//
//		// build packet
//		//
//		sprintf((char*)packet,"%d %d\r\n",CMD_SUBSCRIBE_TO,thrProperties.dataRecvUdpPort);
//		//
//		// send packet
//		//
////		int flag=serviceUdp.send((const unsigned char*)packet, 64, 0);
//		printf("%s subscribe_to %s ipAddress %s port %d\n",thrProperties.name.c_str(),thrProperties.subscribeToName[i].c_str(),
//				thrProperties.subscribeToIpAddress[i].c_str(),thrProperties.subscribeToPort[i]);fflush(stdout);
//		if(!thrProperties.subscribeToFlag[i]) {
//			serviceUdp->select_send((const unsigned char *)packet, 64, 0, i);
//		}
//	}
//}


//void process_packet(uint8_t* packet,char* peerIpAddr,uint16_t peerPort,
//		UdpSocket* serviceUDP,GenericThread* thread,ThreadProperties thrProperties) {
//	uint16_t cmd;
//	uint16_t dataRecvPort;
//	uint8_t ack[64];
//	sscanf((const char*)packet,"%" SCNu16 " %" SCNu16,&cmd,&dataRecvPort);
//	string ipAddr=peerIpAddr;
//	switch(cmd) {
//	case CMD_SUBSCRIBE_TO:
//		// add subscription to thread
//		thread->add_subscription(ipAddr, dataRecvPort);
//		// send acknowledge
//		sprintf((char*)ack,"%d %d\r\n",ACK_SUBSCRIBE_TO,dataRecvPort);
//		serviceUDP->add_peer(peerIpAddr,peerPort);
//		serviceUDP->send(ack, 64, 0);
//		serviceUDP->remove_peer(peerIpAddr, peerPort);
////		serviceUDP->sendto(ack, 64, 0, peerIpAddr, peerPort);
//		printf("%s subscribed ipAddr %s port %d service_peer %d\n",thrProperties.name.c_str(),peerIpAddr,dataRecvPort,peerPort);
//		fflush(stdout);
//		break;
//	case CMD_UNSUBSCRIBE_TO:
//		// remove subscription to thread
//		thread->remove_subscription(ipAddr, dataRecvPort);
//		// send acknowledge
//		sprintf((char*)ack,"%d %d\r\n",ACK_UNSUBSCRIBE_TO,dataRecvPort);
//		serviceUDP->sendto(ack, 64, 0, peerIpAddr, peerPort);
//		printf("%s unsubscribed ipAddr %s port %d service_peer %d\n",thrProperties.name.c_str(),peerIpAddr,dataRecvPort,peerPort);
//		fflush(stdout);
//		break;
//	case ACK_SUBSCRIBE_TO:
//		for(int i=0;i<thrProperties.nSubscribeTo;i++) {
//			if((thrProperties.subscribeToIpAddress[i]==ipAddr)&&(thrProperties.subscribeToPort[i]==peerPort)) {
//				thrProperties.subscribeToFlag[i]=true;
//				printf("%s subscription to %s %s %" PRIu16 " acknowledged\n",
//						thrProperties.name.c_str(),thrProperties.subscribeToName[i].c_str(),ipAddr.c_str(),peerPort);
//			}
//		}
//		break;
//	case ACK_UNSUBSCRIBE_TO:
//		for(int i=0;i<thrProperties.nSubscribeTo;i++) {
//			if((thrProperties.subscribeToIpAddress[i]==ipAddr)&&(thrProperties.subscribeToPort[i]==peerPort)) {
//				thrProperties.subscribeToFlag[i]=false;
//				printf("%s unsubscription to %s %s %" PRIu16 " acknowledged\n",
//						thrProperties.name.c_str(),thrProperties.subscribeToName[i].c_str(),ipAddr.c_str(),peerPort);
//			}
//		}
//		break;
//	case END:
//		running=false;
//		break;
//	default:
//		break;
//	}
//}
