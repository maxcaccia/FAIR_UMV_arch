/*
 * FletcherCheckSum.cpp
 *
 *  Created on: Oct 19, 2023
 *      Author: mc
 */

#include <stdio.h>
#include "FletcherChecksum.h"


uint16_t FletcherChecksum::fletcher_checksum(int packet_length)
{
  // Checksum covers from the first header byte to the last payload byte.
  // This should be equal to the payload length plus the 4 header bytes.
  const int checksum_length = packet_length;

  uint8_t checksum_MSB = 0;
  uint8_t checksum_LSB = 0;

  // Iterate over the packet to compute the checksum.
  for(int i=0; i<checksum_length; i++)
  {
//printf("%x %d %d\n",mPacket[i],i,checksum_length);
    checksum_MSB += mPacket[i];
    checksum_LSB += checksum_MSB;
  }
  return ((uint16_t)checksum_MSB << 8) | (uint16_t)checksum_LSB;
}




