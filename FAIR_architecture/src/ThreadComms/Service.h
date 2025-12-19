/*
 * Service.h
 *
 *  Created on: Nov 30, 2023
 *      Author: mc
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include "GenericThread.h"

//#define CONFIG_PATH "/FAIR_architecture/config/"
//#define CONFIG_PATH "../config/"
#define CONFIG_PATH "../../../FAIR_architecture/config/"

typedef enum {
	//
	// CMD_SUBSCRIBE_TO dataRecvPort
	// cmd parameters
	//	(<peer>ipAddress,<peer>port), needed for ack, are embedded in the UDP packet
	//	dataRecvPort is specified
	//
	CMD_SUBSCRIBE_TO	=	0,
	CMD_UNSUBSCRIBE_TO,
	//
	// ack parameters , i.e. (<peer>ipAddress,<peer>port), are embedded in the UDP packet
	//
	ACK_SUBSCRIBE_TO,
	ACK_UNSUBSCRIBE_TO,
	//
	// terminate the executed thread
	//
	END
} ServiceCmdAck;

//======================================================================================
//
// procedures used by the service process
//
void build_subscribe_to_list(ThreadProperties thrProperties,UdpSocket* serviceUdp);
void subscribe_to(ThreadProperties &thrProperties,UdpSocket* serviceUdp,bool flag=true);
bool process_packet(uint8_t* packet,char* peerIpAddr,uint16_t peerPort,UdpSocket* serviceUDP,GenericThread* thread,ThreadProperties &thrProperties);


#endif /* SERVICE_H_ */
