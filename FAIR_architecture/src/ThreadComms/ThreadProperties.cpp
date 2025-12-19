/*
 * ThreadProperties.cpp
 *
 *  Created on: Oct 25, 2023
 *      Author: mc
 */

#include "ThreadProperties.h"

void ThreadProperties::reset(void) {
	name="NULL";
	moduleClass="NULL";
	moduleName="NULL";
	policy=SCHED_OTHER;
	priority=0;
	moduleType="NULL";
	timeValue=0;
	ipAddress=ANY_ADDR;
	serviceUdpPort=UDP_DEFAULT_SERVICE_PORT;
	cmdRecvUdpPort=UDP_DEFAULT_CMDRECV_PORT;
	dataRecvUdpPort=UDP_DEFAULT_DATARECV_PORT;
	for(int i=0;i<MAX_SOCK_ADDR;i++) {
		commandToName[i]="";
		commandToIpAddress[i]=ANY_ADDR;
		commandToPort[i]=UDP_DEFAULT_CMDRECV_PORT;
		subscribeToName[i]="";
		subscribeToIpAddress[i]=LOCAL_HOST_ADDR;
		subscribeToPort[i]=UDP_DEFAULT_SERVICE_PORT;
		subscribeToFlag[i]=false;
	}
	nCommandTo=0;
	nSubscribeTo=0;
	virtualDeviceName="NULL";
	virtualDeviceIpAddress=LOCAL_HOST_ADDR;
	virtualDevicePort=UDP_DEFAULT_CMDRECV_PORT;
	actualDeviceType="NULL";
	actualDeviceIpAddress=LOCAL_HOST_ADDR;
	actualDevicePort=UDP_DEFAULT_CMDRECV_PORT;
	actualDeviceTty="";
	rs232blocking=false;
	rs232canonicalMode=false;
	rs232baudRate=B115200;
	rs232parityEnable=0;
	rs232parityType=0;
	rs232stopBits=0;
	rs232csNumber=CS8;
}

ThreadProperties & ThreadProperties::operator = (const ThreadProperties &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	configDir=obj.configDir;
	name=obj.name;
	moduleClass=obj.moduleClass;
	moduleName=obj.moduleName;
	policy=obj.policy;
	priority=obj.priority;
	moduleType=obj.moduleType;
	timeValue=obj.timeValue;
	ipAddress=obj.ipAddress;
	serviceUdpPort=obj.serviceUdpPort;
	cmdRecvUdpPort=obj.cmdRecvUdpPort;
	dataRecvUdpPort=obj.dataRecvUdpPort;
	for(int i=0;i<MAX_SOCK_ADDR;i++) {
		commandToName[i]=obj.commandToName[i];
		commandToIpAddress[i]=obj.commandToIpAddress[i];
		commandToPort[i]=obj.commandToPort[i];
		subscribeToName[i]=obj.subscribeToName[i];
		subscribeToIpAddress[i]=obj.subscribeToIpAddress[i];
		subscribeToPort[i]=obj.subscribeToPort[i];
		subscribeToFlag[i]=obj.subscribeToFlag[i];
	}
	nCommandTo=obj.nCommandTo;
	nSubscribeTo=obj.nSubscribeTo;
	virtualDeviceName=obj.virtualDeviceName;
	virtualDeviceIpAddress=obj.virtualDeviceIpAddress;
	virtualDevicePort=obj.virtualDevicePort;
	actualDeviceType=obj.actualDeviceType;
	actualDeviceIpAddress=obj.actualDeviceIpAddress;
	actualDevicePort=obj.actualDevicePort;
	actualDeviceTty=obj.actualDeviceTty;
	rs232blocking=obj.rs232blocking;
	rs232canonicalMode=obj.rs232canonicalMode;
	rs232baudRate=obj.rs232baudRate;
	rs232parityEnable=obj.rs232parityEnable;
	rs232parityType=obj.rs232parityType;
	rs232stopBits=obj.rs232stopBits;
	rs232csNumber=obj.rs232csNumber;

	// return a reference to myself
	return *this;
}

bool ThreadProperties::fread(const char *fileName) {
	FILE *f;
	string str;
	str=configDir+fileName;

	if((f=fopen(str.c_str(),"r"))==NULL) {
		printf("ThreadProperties::fread ERROR - cannot open file %s %s\n",fileName,str.c_str());
		return false;
	}

	if(!fread_common(f)) {
		printf("ThreadProperties::fread ERROR - fread_common() fails\n");
		fclose(f);
		return false;
	}

	if((moduleClass=="CNTRL_ARCH_MODULE")||(moduleClass=="SIMULATOR")) {
		if(!fread_cntrl_arch_module(f)) {
				printf("ThreadProperties::fread ERROR - fread_cntrl_arch_module() fails\n");
				fclose(f);
				return false;
			}
	}
	else if(moduleClass=="DRIVER") {
		if(!fread_driver(f)) {
				printf("ThreadProperties::fread ERROR - fread_driver() fails\n");
				fclose(f);
				return false;
			}
	}
	else {
		printf("ThreadProperties::fread ERROR - module class %s unknown\n",moduleClass.c_str());
		fclose(f);
		return false;
	}

	fclose(f);
	return true;
}

bool ThreadProperties::fread_common(FILE *f) {
	string str;
	string value;
	int number;
	// read separator line
	fread_string(f,str);
	// read name
	fread_string(f,str); fread_string(f,value);
	if(str!="#name") {
		printf("ThreadProperties::fread_common ERROR - #name expected %s found\n",str.c_str());
		return false;
	}
	name=value;
	// read separator line
	fread_string(f,str);
	// read class
	fread_string(f,str); fread_string(f,value);
	if(str!="#class") {
		printf("ThreadProperties::fread_common ERROR - #class expected %s found\n",str.c_str());
		return false;
	}
	moduleClass=value;
	// read module
	fread_string(f,str); fread_string(f,value);
	if(str!="#module") {
		printf("ThreadProperties::fread_common ERROR - #module expected %s found\n",str.c_str());
		return false;
	}
	moduleName=value;
	// read policy
	fread_string(f,str); fread_string(f,value);
	if(str!="#policy") {
		printf("ThreadProperties::fread_common ERROR - #policy expected %s found\n",str.c_str());
		return false;
	}
	if(value=="SCHED_FIFO") policy=SCHED_FIFO;
	else if(value=="SCHED_OTHER") policy=SCHED_OTHER;
	else {
		printf("ThreadProperties::fread_common ERROR - wrong policy %s\n",value.c_str());
		return false;
	}
	// read priority
	fread_string(f,str); fscanf(f,"%d",&number);
	if(str!="#priority") {
		printf("ThreadProperties::fread_common ERROR - #priority expected %s found\n",str.c_str());
		return false;
	}
	priority=number;
	// read type
	fread_string(f,str); fread_string(f,value);
	if(str!="#type") {
		printf("ThreadProperties::fread_common ERROR - #type expected %s found\n",str.c_str());
		return false;
	}
	if((value!="SYNCHRONOUS")&&(value!="ASYNCHRONOUS")) {
		printf("ThreadProperties::fread_common ERROR - wrong type %s\n",value.c_str());
		return false;
	}
	moduleType=value;
	// read time value
	fread_string(f,str); fscanf(f,"%d",&number);
	if(str!="#time_value") {
		printf("ThreadProperties::fread_common ERROR - #time_value expected %s found\n",str.c_str());
		return false;
	}
	timeValue=number;
	// read IP address
	fread_string(f,str); fread_string(f,value);
	if(str!="#ip_address") {
		printf("ThreadProperties::fread_common ERROR - #ip_address expected %s found\n",str.c_str());
		return false;
	}
	ipAddress=value;
	// read service UDP port
	fread_string(f,str); fscanf(f,"%d",&number);
	if(str!="#serviceUdpPort") {
		printf("ThreadProperties::fread_common ERROR - #serviceUdpPort expected %s found\n",str.c_str());
		return false;
	}
	serviceUdpPort=number;
	// read cmdRecv UDP port
	fread_string(f,str); fscanf(f,"%d",&number);
	if(str!="#cmdRecvUdpPort") {
		printf("ThreadProperties::fread_common ERROR - #cmdRecvUdpPort expected %s found\n",str.c_str());
		return false;
	}
	cmdRecvUdpPort=number;
	// read dataRecvUdpPort UDP port
	fread_string(f,str); fscanf(f,"%d",&number);
	if(str!="#dataRecvUdpPort") {
		printf("ThreadProperties::fread_common ERROR - #dataRecvUdpPort expected %s found\n",str.c_str());
		return false;
	}
	dataRecvUdpPort=number;

	return true;
}

bool ThreadProperties::fread_cntrl_arch_module(FILE *f) {
	string str;
	bool flag;
	//
	// read "command to" list
	//
	// read command_to
	fread_string(f,str);
	if(str!="#command_to") {
		printf("ThreadProperties::fread_common ERROR - #command_to expected %s found\n",str.c_str());
		return false;
	}
	// read list
	flag=true;
	while(flag) {
		// read name
		fread_string(f,str);
		if(str=="#end")
			flag=false;
		else
			commandToName[nCommandTo++]=str;
	}
	//
	// read "subscribe to" list
	//
	// read subscribe_to
	fread_string(f,str);
	if(str!="#subscribe_to") {
		printf("ThreadProperties::fread_common ERROR - #subscribe_to expected %s found\n",str.c_str());
		return false;
	}
	// read list
	flag=true;
	while(flag) {
		// read name
		fread_string(f,str);
		if(str=="#end")
			flag=false;
		else
			subscribeToName[nSubscribeTo++]=str;
	}

	return true;
}


bool ThreadProperties::fread_driver(FILE *f) {
	string str;
	string value;
	int number;
	// read virtual device
	fread_string(f,str); fread_string(f,value);
	if(str!="#virtual_device") {
		printf("ThreadProperties::fread_driver ERROR - #virtual_device expected %s found\n",str.c_str());
		return false;
	}
	virtualDeviceName=value;
//	commandToName[nCommandTo++]=value;
	subscribeToName[nSubscribeTo++]=value;
	// read actual device
	fread_string(f,str); fread_string(f,value);
	if(str!="#actual_device") {
		printf("ThreadProperties::fread_driver ERROR - #actual_device expected %s found\n",str.c_str());
		return false;
	}
	actualDeviceType=value;
	if(actualDeviceType=="RS-232") {
		// read tty
		fread_string(f,str); fread_string(f,value);
		if(str!="#tty") {
			printf("ThreadProperties::fread_driver ERROR - #tty expected %s found\n",str.c_str());
			return false;
		}
		actualDeviceTty=value;
		// read rs232blocking
		fread_string(f,str); fread_string(f,value);
		if(str!="#rs232blocking") {
			printf("ThreadProperties::fread_driver ERROR - #rs232blocking expected %s found\n",str.c_str());
			return false;
		}
		if(value=="false")
			rs232blocking = false;
		else if(value=="true")
			rs232blocking = true;
		else {
			printf("ThreadProperties::fread_driver ERROR - false/true expected %s found\n",value.c_str());
			return false;
		}
		// read rs232canonicalMode
		fread_string(f,str); fread_string(f,value);
		if(str!="#rs232canonicalMode") {
			printf("ThreadProperties::fread_driver ERROR - #rs232canonicalMode expected %s found\n",str.c_str());
			return false;
		}
		if(value=="false")
			rs232canonicalMode = false;
		else if(value=="true")
			rs232canonicalMode = true;
		else {
			printf("ThreadProperties::fread_driver ERROR - false/true expected %s found\n",value.c_str());
			return false;
		}
		// read rs232baudRate
		fread_string(f,str); fread_string(f,value);
		if(str!="#rs232baudRate") {
			printf("ThreadProperties::fread_driver ERROR - #rs232baudRate expected %s found\n",str.c_str());
			return false;
		}
		if(value=="B9600")
			rs232baudRate = B9600;
		else if(value=="B19200")
			rs232baudRate = B19200;
		else if(value=="B38400")
			rs232baudRate = B38400;
		else if(value=="B57600")
			rs232baudRate = B57600;
		else if(value=="B115200")
			rs232baudRate = B115200;
		else {
			printf("ThreadProperties::fread_driver ERROR - #rs232baudRate %s not supported\n",value.c_str());
			return false;
		}
		// read rs232parityEnable
		fread_string(f,str); fscanf(f,"%d",&number);
		if(str!="#rs232parityEnable") {
			printf("ThreadProperties::fread_driver ERROR - #rs232parityEnable expected %s found\n",str.c_str());
			return false;
		}
		rs232parityEnable=number;
		// read rs232parityType
		fread_string(f,str); fscanf(f,"%d",&number);
		if(str!="#rs232parityType") {
			printf("ThreadProperties::fread_driver ERROR - #rs232parityType expected %s found\n",str.c_str());
			return false;
		}
		rs232parityType=number;
		// read rs232stopBits
		fread_string(f,str); fscanf(f,"%d",&number);
		if(str!="#rs232stopBits") {
			printf("ThreadProperties::fread_driver ERROR - #rs232stopBits expected %s found\n",str.c_str());
			return false;
		}
		rs232stopBits=number;
		// read rs232csNumber
		fread_string(f,str); fread_string(f,value);
		if(str!="#rs232csNumber") {
			printf("ThreadProperties::fread_driver ERROR - #rs232csNumber expected %s found\n",str.c_str());
			return false;
		}
		if(value=="CS5")
			rs232csNumber = CS5;
		if(value=="CS6")
			rs232csNumber = CS6;
		if(value=="CS7")
			rs232csNumber = CS7;
		if(value=="CS8")
			rs232csNumber = CS8;
		else {
			printf("ThreadProperties::fread_driver ERROR - #rs232csNumber %s not supported\n",value.c_str());
			return false;
		}
	}
	else if(actualDeviceType=="UDP_SOCKET") {
		// read device IP address
		fread_string(f,str); fread_string(f,value);
		if(str!="#device_ip_address") {
			printf("ThreadProperties::fread_driver ERROR - #device_ip_address expected %s found\n",str.c_str());
			return false;
		}
		actualDeviceIpAddress=value;
		// read port
		fread_string(f,str); fscanf(f,"%d",&number);
		if(str!="#device_port") {
			printf("ThreadProperties::fread_driver ERROR - #device_port expected %s found\n",str.c_str());
			return false;
		}
		actualDevicePort=number;
	}
	else {
		printf("ThreadProperties::fread_driver ERROR - #actual_device type %s unknown\n",actualDeviceType.c_str());
		return false;
	}

	return true;
}


void ThreadProperties::print(void) {
	print_common();
	if((moduleClass=="CNTRL_ARCH_MODULE")||(moduleClass=="SIMULATOR"))
		print_cntrl_arch_fields();
	if(moduleClass=="DRIVER")
		print_driver_fields();
}



void ThreadProperties::print_common(void) {
	printf("#name %s\n",name.c_str());
	printf("#class %s\n",moduleClass.c_str());
	printf("#module %s\n",moduleName.c_str());
	printf("#policy %d\n",policy);
	printf("#priority %d\n",priority);
	printf("#type %s\n",moduleType.c_str());
	printf("#time_value %d\n",timeValue);
	printf("#ip_address %s\n",ipAddress.c_str());
	printf("#serviceUdpPort %d\n",serviceUdpPort);
	printf("#cmdRecvUdpPort %d\n",cmdRecvUdpPort);
	printf("#dataRecvUdpPort %d\n",dataRecvUdpPort);
	fflush(stdout);
}

void ThreadProperties::print_cntrl_arch_fields(void) {
	printf("#command_to\n");
	for(int i=0;i<nCommandTo;i++) {
		printf("\t%s\n",commandToName[i].c_str());
		printf("\t\t%s\n",commandToIpAddress[i].c_str());
		printf("\t\t%d\n",commandToPort[i]);
	}
	printf("#subscribe_to\n");
	for(int i=0;i<nSubscribeTo;i++) {
		printf("\t%s\n",subscribeToName[i].c_str());
		printf("\t\t%s\n",subscribeToIpAddress[i].c_str());
		printf("\t\t%d\n",subscribeToPort[i]);
	}
	fflush(stdout);
}

void ThreadProperties::print_driver_fields(void) {
	print_cntrl_arch_fields();
	printf("#virtual_device %s\n",virtualDeviceName.c_str());
	printf("#virtual_device_ip_address %s\n",virtualDeviceIpAddress.c_str());
	printf("#virtual_device_port %d\n",virtualDevicePort);
	printf("#actual_device %s\n",actualDeviceType.c_str());
	printf("#device_ip_address %s\n",actualDeviceIpAddress.c_str());
	printf("#device_port %d\n",actualDevicePort);
	printf("#tty %s\n",actualDeviceTty.c_str());
	printf("#rs232blocking %d\n",(int)rs232blocking);
	printf("#rs232canonicalMode %d\n",(int)rs232canonicalMode);
	printf("#rs232baudRate %" PRIu32 "\n",rs232baudRate);
	printf("#rs232parityEnable %" PRIu32 "\n",rs232parityEnable);
	printf("#rs232parityType %" PRIu32 "\n",rs232parityType);
	printf("#rs232stopBits %" PRIu32 "\n",rs232stopBits);
	printf("#rs232csNumber %" PRIu32 "\n",rs232csNumber);
	fflush(stdout);
}

bool ThreadProperties::process_dependencies(void) {
	ThreadProperties tmp(this->configDir);
	string str;

	//
	// process "command_to" dependencies
	//
	for(int i=0;i<this->nCommandTo;i++) {
		str = this->commandToName[i] +".thread";
		if(tmp.fread(str.c_str())) {
			this->commandToIpAddress[i]=tmp.ipAddress;
			this->commandToPort[i]=tmp.cmdRecvUdpPort;
		}
		else {
			printf("ThreadProperties::process_dependencies ERROR - module %s unknown\n",this->commandToName[i].c_str());
			return false;
		}
	}
	//
	// process "subscribe_to" dependencies
	//
	for(int i=0;i<this->nSubscribeTo;i++) {
		str = this->subscribeToName[i] +".thread";
		if(tmp.fread(str.c_str())) {
			this->subscribeToIpAddress[i]=tmp.ipAddress;
			this->subscribeToPort[i]=tmp.serviceUdpPort;
			this->subscribeToFlag[i]=true;
		}
		else {
			printf("ThreadProperties::process_dependencies ERROR - module %s unknown\n",this->subscribeToName[i].c_str());
			return false;
		}
	}
	//
	// process driver - virtual device dependencies
	//
	if(this->moduleClass=="DRIVER") {
		str = this->virtualDeviceName +".thread";
		if(tmp.fread(str.c_str())) {
			this->virtualDeviceIpAddress=tmp.ipAddress;
			this->virtualDevicePort=tmp.cmdRecvUdpPort;
		}
		else {
			printf("ThreadProperties::process_dependencies ERROR - module %s unknown\n",this->virtualDeviceName.c_str());
			return false;
		}

	}

	return true;
}
