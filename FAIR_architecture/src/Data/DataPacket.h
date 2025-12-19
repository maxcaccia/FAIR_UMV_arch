/*
 * DataPacket.h
 *
 *  Created on: Oct 20, 2023
 *      Author: mc
 */

#ifndef DATAPACKET_H_
#define DATAPACKET_H_

#include <inttypes.h>
#include <stdio.h>

#define MAX_PACKET_LENGTH 1024

class DataPacket {
public:
	DataPacket()  {clean();}
	~DataPacket() {}

	void clean(void) {for(int i=0;i<MAX_PACKET_LENGTH;i++) mData[i]=0x00; m_n=0;}
	void append_byte(uint8_t byte) {mData[m_n++]=byte;}

//	void print() { printf("DataPacket::print\n"); for(int i=0;i<m_n;i++){ printf("%x ",mData[i]);} printf("\n"); }
	void print() { printf("DataPacket::print\n"); for(int i=0;i<m_n;i++){ printf("%c",mData[i]);} printf("\n"); }

	uint8_t mData[MAX_PACKET_LENGTH];
	int m_n;
};

#endif /* DATAPACKET_H_ */
