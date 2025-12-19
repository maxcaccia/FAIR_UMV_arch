/*
 * DataParser.cpp
 *
 *  Created on: Oct 17, 2023
 *      Author: mc
 */

#include "DataParser.h"

bool DataParser::execute(uint8_t byte) {
	bool flag=false;
	if(process_byte(byte)) {
//		printf("DataParser::execute\n");
//		mDataPacket.print();
		flag=decode_packet();
		if(flag)
			update_data();
		reset();
	}
	return flag;
}

bool DataParser::execute(uint8_t* packet,int length) {
	for(int i=0;i<length;i++)
		mDataPacket.append_byte(packet[i]);
	bool flag=false;
	flag=decode_packet();
	if(flag)
		update_data();
	reset();
	return flag;
}


bool DataParser::process_byte(uint8_t byte) {
	bool flag=false;
	if(!mHeaderDetected)
		mHeaderDetected=detect_header(byte);
	else
		flag=build_packet(byte);
	return flag;
}

