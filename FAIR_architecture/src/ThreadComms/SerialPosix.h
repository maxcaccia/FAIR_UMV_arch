/*
 * SerialPosix.h
 *
 *  Created on: May 31, 2023
 *      Author: mc
 *
 *  Code inspired by https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
 *
 */

#ifndef SERIALPOSIX_H_
#define SERIALPOSIX_H_

#include <stdio.h>
#include <string>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include "inttypes.h"
#include "string.h"

using namespace std;

struct SerialPosixPar {
	string ttyName;
	bool blocking;
	bool canonicalMode;
	uint32_t baudRate;
	uint32_t parityEnable;
	uint32_t parityType;
	uint32_t stopBits;
	uint32_t csNumber;
};

class SerialPosix {
	string mDeviceName;
	string mTtyName;
	int serial_port;
	struct termios tty={0};
	bool mBlocking;
	bool mCanonicalMode;
	uint32_t mBaudRate;
	uint32_t mParityEnable;
	uint32_t mParityType;
	uint32_t mStopBits;
	uint32_t mCsNumber;

public:
	SerialPosix(string deviceName,SerialPosixPar *par);
	~SerialPosix(void) {if(serial_port>=0) close(serial_port);};

	bool create(void);
	void end() {if(serial_port>=0) close(serial_port); serial_port=-1;};

	ssize_t get_data(void *read_buf,size_t buf_size) { return(read(serial_port,read_buf,buf_size)); }

private:
	bool check_parameters(uint32_t baudRate,uint32_t parityEnable,uint32_t parityType,uint32_t stopBits,uint32_t csNumber);
//	bool check_parameters(SerialPosixPar serialPosixPar);
};



#endif /* SERIALPOSIX_H_ */
