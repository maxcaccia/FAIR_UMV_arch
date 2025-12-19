/*
 * Service.cpp
 *
 *  Created on: Dec 19, 2023
 *      Author: mc
 */

#include "Service.h"

//======================================================================================
//
// procedures used by the service process
//
void build_subscribe_to_list(ThreadProperties thrProperties,UdpSocket* serviceUdp) {
	for(int i=0;i<thrProperties.nSubscribeTo;i++)
	{
		char address[64];
		strcpy(address,thrProperties.subscribeToIpAddress[i].c_str());
		serviceUdp->add_peer(address, thrProperties.subscribeToPort[i]);
	}
}


void subscribe_to(ThreadProperties &thrProperties,UdpSocket* serviceUdp,bool flag) {
	if(!flag) return;
	for(int i=0;i<thrProperties.nSubscribeTo;i++) {
		uint8_t packet[64];
		//
		// build packet
		//
		sprintf((char*)packet,"%d %d\r\n",CMD_SUBSCRIBE_TO,thrProperties.dataRecvUdpPort);fflush(stdout);
		//
		// send packet
		//
		if(thrProperties.subscribeToFlag[i]) {
			printf("%s subscribe_to %s ipAddress %s port %d\n",thrProperties.name.c_str(),thrProperties.subscribeToName[i].c_str(),
					thrProperties.subscribeToIpAddress[i].c_str(),thrProperties.subscribeToPort[i]);fflush(stdout);
			serviceUdp->select_send((const unsigned char *)packet, 64, 0, i);
		}
	}
}


bool process_packet(uint8_t* packet,char* peerIpAddr,uint16_t peerPort,
		UdpSocket* serviceUDP,GenericThread* thread,ThreadProperties &thrProperties) {
	bool flag=true;
//printf("process_packet peerIpAddr %s peerPort %" PRIu16 " packet %s\n",peerIpAddr,peerPort,(char*)packet);
	uint16_t cmd;
	uint16_t dataRecvPort;
	uint8_t ack[64];
	sscanf((const char*)packet,"%" SCNu16 " %" SCNu16,&cmd,&dataRecvPort);
	string ipAddr=peerIpAddr;
	switch(cmd) {
	case CMD_SUBSCRIBE_TO:
		// add subscription to thread
		thread->add_subscription(ipAddr, dataRecvPort);
		// send acknowledge
		sprintf((char*)ack,"%d %d\r\n",ACK_SUBSCRIBE_TO,dataRecvPort);
		serviceUDP->add_peer(peerIpAddr,peerPort);
		serviceUDP->send(ack, 64, 0);
		serviceUDP->remove_peer(peerIpAddr, peerPort);
//		serviceUDP->sendto(ack, 64, 0, peerIpAddr, peerPort);
		printf("%s subscribed ipAddr %s port %d service_peer %d\n",thrProperties.name.c_str(),peerIpAddr,dataRecvPort,peerPort);
		fflush(stdout);
		break;
	case CMD_UNSUBSCRIBE_TO:
		// remove subscription to thread
		thread->remove_subscription(ipAddr, dataRecvPort);
		// send acknowledge
		sprintf((char*)ack,"%d %d\r\n",ACK_UNSUBSCRIBE_TO,dataRecvPort);
		serviceUDP->sendto(ack, 64, 0, peerIpAddr, peerPort);
		printf("%s unsubscribed ipAddr %s port %d service_peer %d\n",thrProperties.name.c_str(),peerIpAddr,dataRecvPort,peerPort);
		fflush(stdout);
		break;
	case ACK_SUBSCRIBE_TO:
		for(int i=0;i<thrProperties.nSubscribeTo;i++) {
//printf("%d %s %s %d %d\n",i,thrProperties.subscribeToIpAddress[i].c_str(),ipAddr.c_str(),thrProperties.subscribeToPort[i],peerPort);
//fflush(stdout);
			if((thrProperties.subscribeToIpAddress[i]==ipAddr)&&(thrProperties.subscribeToPort[i]==peerPort)) {
				thrProperties.subscribeToFlag[i]=false;
				printf("%s subscription to %s %s %" PRIu16 " acknowledged\n",
						thrProperties.name.c_str(),thrProperties.subscribeToName[i].c_str(),ipAddr.c_str(),peerPort);
				fflush(stdout);
			}
		}
		break;
	case ACK_UNSUBSCRIBE_TO:
		for(int i=0;i<thrProperties.nSubscribeTo;i++) {
			if((thrProperties.subscribeToIpAddress[i]==ipAddr)&&(thrProperties.subscribeToPort[i]==peerPort)) {
				thrProperties.subscribeToFlag[i]=false;
				printf("%s unsubscription to %s %s %" PRIu16 " acknowledged\n",
						thrProperties.name.c_str(),thrProperties.subscribeToName[i].c_str(),ipAddr.c_str(),peerPort);
			}
		}
		break;
	case END:
		flag=false;
		break;
	default:
		break;
	}
	return flag;
}

