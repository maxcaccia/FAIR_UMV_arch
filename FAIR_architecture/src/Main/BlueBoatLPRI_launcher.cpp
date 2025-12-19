/*
 * BlueBoatLPRI_launcher.cpp
 *
 *  Created on:30 July 2025
 *      Author: Massimo Caccia
 */
#include <signal.h>

#include <string>

#include "Service.h"
#include "ThreadProperties.h"

//***** insert your code here ************************************
#include "BlueBoatLPRIthread.h"
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
//======================================================================================
//
// main
//	called as: <module>_launcher <threadName> <serviceUDPtimeOut>
//		where
//			<serviceUDPtimeOut> is in [s]
//
//======================================================================================
int main(int argc, char* argv[])
{
	//======================================================================================
	//
	// check argc
	//
	if (argc < 3) {
		// <threadName> and <serviceUDPtimeOut> are always requested
		printf("Wrong parameter number\n\t<module>_launcher  <threadName> <serviceUDPtimeOut>\n");
		fflush(stdout);
		return -1;
	}

	//======================================================================================
	//
	// check help
	//
	if (strcmp(argv[1],"help")==0) {
		printf("<module>_launcher <threadName> <serviceUDPtimeOut>\n");
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
	if(thrProperties.moduleName=="BLUEBOAT_LPRI") {
		genericThread = new BlueBoatLPRIthread();
	}
	else {
		printf("ERROR: BlueBoatLPRIthread not implemented!\n"); fflush(stdout);
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
		//
		// subscribe to modules
		//
		subscribe_to(thrProperties,&serviceUDP);
		//
		// receive command/acknowledgments
		//
		if(serviceUDP.recvfrom(packet, 64, 0,peerIpAddr,peerPort)!=-1) {
			//
			// process command/acknowledgments
			//
			process_packet(packet,peerIpAddr,peerPort,&serviceUDP,genericThread,thrProperties);
		}
//		printf("=========================================== serviceUDP.recvfrom time out\n");fflush(stdout);
	}

	genericThread->end();

	printf("launcher ends...\n");

	return 0;
}

