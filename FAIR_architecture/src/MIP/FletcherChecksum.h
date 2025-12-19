/*
 * FletcherChecksum.h
 *
 *  Created on: Oct 19, 2023
 *      Author: mc
 */

#ifndef FLETCHERCHECKSUM_H_
#define FLETCHERCHECKSUM_H_

//#include <stdio.h>
#include <inttypes.h>

#define FLETCHER_MAX_PACKET_SIZE 256

class FletcherChecksum {
public:
	FletcherChecksum(void) { clean_checksum(); }
	~FletcherChecksum() {}

	void add_byte(uint8_t byte) { mPacket[mIndex++]=byte; }
	void clean_checksum(void) { for(int i=0;i<FLETCHER_MAX_PACKET_SIZE;i++) mPacket[i]=0; mIndex=0;}
	uint16_t fletcher_checksum(int packetLength);

private:
	int mIndex;
	uint8_t mPacket[FLETCHER_MAX_PACKET_SIZE];
};



#endif /* FLETCHERCHECKSUM_H_ */
