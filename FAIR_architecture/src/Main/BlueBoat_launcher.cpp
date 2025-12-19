/*
 * BlueBoat_launcher.cpp
 *
 *  Created on: 23 March 2025
 *      Author: Massimo Caccia
 */
#include <signal.h>

#include <string>

#include "Service.h"

//***** insert your code here ************************************
//
// #include "<module>thread.h"
//
#include "BlueBoatThread.h"
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
//	called as: main_launcher_<module> <threadName> <serviceUDPtimeOut> <true/false>
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
		// <threadName> and <serviceUDPtimeOut> are always requested
		printf("Wrong parameter number\n\tmain_launcher_<module>  <threadName> <serviceUDPtimeOut> <true/false>\n");
		fflush(stdout);
		return -1;
	}

	//======================================================================================
	//
	// check help
	//
	if (strcmp(argv[1],"help")==0) {
		printf("<module>_launcher <threadName> <serviceUDPtimeOut> <true/false>\n");
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
//	bool flag;
//	if(thrProperties.moduleClass!="DRIVER")
//		flag=true;
//	else {
//		if((thrProperties.moduleClass=="DRIVER")&&(argc==4)) {
//			string str=argv[3];
//			if(str=="true")
//				flag=false;
//			else
//				flag=true;
//			printf("-------------------- flag %d\n",(int)flag);fflush(stdout);
//		}
//	}
//	if(flag)
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
	if(thrProperties.moduleName=="BLUEBOAT") {
		genericThread = new BlueBoatThread();
	}
	else {
		printf("ERROR: BlueBoatThread not implemented!\n"); fflush(stdout);
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
	bool subscribeToFlag=true;
	if((thrProperties.moduleClass=="DRIVER")&&(argc==4)) {
		string flag=argv[3];
		if(flag=="true") {
			isActual=true;
			subscribeToFlag=false;
		}
		//***** insert your code here ************************************
		//
		// ((<module>thread*)genericThread)->set_is_actual(isActual);
		//
		((BlueBoatThread*)genericThread)->set_is_actual(isActual);
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
	printf("\n=========================================""\n=========================================\n\n");fflush(stdout);
	printf("%s in execution ...\n",thrProperties.name.c_str());fflush(stdout);
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
		subscribe_to(thrProperties,&serviceUDP,subscribeToFlag);
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

