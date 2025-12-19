/*
 * Packet.cpp
 *
 *  Created on: 14 February 2024
 *      Author: massimo
 */

#include "Packet.h"

Packet & Packet::operator = (const Packet &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	n=obj.n;
	for(int i=0;i<PACKET_SIZE;i++)
		data[i]=obj.data[i];
	// return a reference to myself
	return *this;
}

