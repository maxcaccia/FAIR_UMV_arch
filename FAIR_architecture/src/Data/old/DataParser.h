/*
 * DataParser.h
 *
 *  Created on: Oct 17, 2023
 *      Author: Massimo Caccia
 *
 *
 */

#ifndef DATAPARSER_H_
#define DATAPARSER_H_

#include <stdio.h>
#include <inttypes.h>

#include "DataPacket.h"
#include "TimeStampedData.h"

class DataParser {
public:
	DataParser(void) { mHeaderDetected=false;}
	virtual ~DataParser() {};

	bool execute(uint8_t byte);	// return true when a correct packet is read
	bool execute(uint8_t* packet,int length); // return true when a correct packet is decoded

	virtual void get_data(TimeStampedData &data) {printf("virtual DataParser::get_data \n");}

protected:
	bool process_byte(uint8_t byte);	// return true when a packet is read
	virtual bool detect_header(uint8_t byte) {printf("virtual DataParser::detect_header %" PRIu8 "\n",byte);return true;}
	virtual bool build_packet(uint8_t byte) {printf("virtual DataParser::detect_header %" PRIu8 "\n",byte);return true;}
	virtual bool decode_packet(void)  {printf("virtual DataParser::decode_packet \n");return true;}	// decode packet data, called when process_byte(.) returns true if packet is correct
	virtual void reset(void)  {mHeaderDetected=false;mDataPacket.clean();}

	virtual void update_data(void) {printf("virtual DataParser::update_data \n");}

protected:
	DataPacket mDataPacket;
private:
	bool mHeaderDetected;
};

#endif /* DATAPARSER_H_ */
