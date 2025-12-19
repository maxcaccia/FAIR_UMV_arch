/*
 * ThreadProperties.h
 *
 *  Created on: Oct 25, 2023
 *      Author: mc
 */

#ifndef THREADPROPERTIES_H_
#define THREADPROPERTIES_H_

#include <string>
using namespace std;

#include <sched.h>
#include <termios.h> // Contains POSIX terminal control definitions
#include <inttypes.h>

#include "SocketDefine.h"
//#include "SerialPosix.h"

#define UDP_DEFAULT_SERVICE_PORT	99
#define UDP_DEFAULT_CMDRECV_PORT	98
#define UDP_DEFAULT_DATARECV_PORT	97

class ThreadProperties {
public:
	//
	// generic thread properties
	//
	string configDir;
	string name;
	string moduleClass;
	string moduleName;
	int policy;
	int priority;
	string moduleType;
	int timeValue;
	string ipAddress;
	int serviceUdpPort;
	int cmdRecvUdpPort;
	int dataRecvUdpPort;
	//
	// cntrl/sim thread properties
	//
	string commandToName[MAX_SOCK_ADDR];
	string commandToIpAddress[MAX_SOCK_ADDR];
	int commandToPort[MAX_SOCK_ADDR];
	int nCommandTo;
	string subscribeToName[MAX_SOCK_ADDR];
	string subscribeToIpAddress[MAX_SOCK_ADDR];
	int subscribeToPort[MAX_SOCK_ADDR];
	bool subscribeToFlag[MAX_SOCK_ADDR];
	int nSubscribeTo;
	//
	// driver thread properties
	//
	string virtualDeviceName;
	string virtualDeviceIpAddress;
	int virtualDevicePort;
	string actualDeviceType;
	string actualDeviceIpAddress;
	int actualDevicePort;
	string actualDeviceTty;
	bool rs232blocking;
	bool rs232canonicalMode;
	uint32_t rs232baudRate;
	uint32_t rs232parityEnable;
	uint32_t rs232parityType;
	uint32_t rs232stopBits;
	uint32_t rs232csNumber;

public:
	ThreadProperties(string configDirName) { configDir=configDirName; reset(); };
	ThreadProperties(void) { reset(); }
	~ThreadProperties(void) { }

	ThreadProperties & operator = (const ThreadProperties &obj);

	bool fread(const char *fileName);
	bool fread_common(FILE *f);
	bool fread_cntrl_arch_module(FILE *f);
	bool fread_driver(FILE *f);
	void fread_string(FILE *f,string &str) {char s[256];fscanf(f,"%s",s);str=s;}

	bool process_dependencies(void);

	void print(void);
	void print_common(void);
	void print_cntrl_arch_fields(void);
	void print_driver_fields(void);

private:
	void reset(void);
};



#endif /* THREADPROPERTIES_H_ */
