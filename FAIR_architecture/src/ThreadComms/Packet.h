/*
 * Packet.h
 *
 *  Created on: 14 February 2024
 *      Author: massimo
 */
#ifndef PACKET_H_
#define PACKET_H_

#include <inttypes.h>
//#include <stdio.h>
//#include <string.h>
//#include <time.h>
//#include <math.h>

//#define PACKET_SIZE 4096
#define PACKET_SIZE 4096


class Packet {
public:
	uint8_t data[PACKET_SIZE];
//	uint8_t data[4096];
	int n;

public:
	Packet(void) {n=0;/*reset();*/}
    ~Packet(void) {};

    Packet & operator = (const Packet &obj);

    void reset(void) { for(int i=0;i<PACKET_SIZE;i++) data[i]=0; n=0; }
};

#endif /* PACKET_H_ */
