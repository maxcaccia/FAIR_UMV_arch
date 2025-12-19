/*
 * SerialPosix.cpp
 *
 *  Created on: May 31, 2023
 *      Author: mc
 *
 *  Code inspired by https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
 *
 */

#include "SerialPosix.h"

SerialPosix::SerialPosix(string deviceName,SerialPosixPar *par) {
	mDeviceName=deviceName;
	mTtyName=par->ttyName;
	serial_port=-1;
	mBlocking=par->blocking;
	mCanonicalMode=par->canonicalMode;
	mBaudRate=par->baudRate;
	mParityEnable=par->parityEnable;
	mParityType=par->parityType;
	mStopBits=par->stopBits;
	mCsNumber=par->csNumber;
}

//size_t SerialPosix::get_data(void *read_buf,size_t buf_size) {
//	char packet[256];
////	memset(&packet, '\0', 256);
//	int n;
//	n=read(serial_port,packet,sizeof(packet));
//	printf("n %d packet %s\n",n,packet);
//
//	return(0);
//}

bool SerialPosix::create(void) {
	//
	// check serial init parameters
	//
	if(!check_parameters(mBaudRate,mParityEnable,mParityType,mStopBits,mCsNumber)) {
		printf("ERROR - SerialPosix::SerialPosix() %s : error check_parameters(...)\n",mDeviceName.c_str());
		return false;
	}
	//
	// open device
	//
	if(mBlocking)
		serial_port = open(mTtyName.c_str(), O_RDWR); // Canonical mode: blocking
	else
		serial_port = open(mTtyName.c_str(), O_RDWR|O_NONBLOCK); // Canonical mode: non blocking
	if(serial_port<0) {
		printf("ERROR - SerialPosix::SerialPosix()  %s %s: error %i from open: %s\n",
				mDeviceName.c_str(),mTtyName.c_str(),errno, strerror(errno));
		return false;
	}
	// Read in existing settings, and handle any error
	// NOTE: This is important! POSIX states that the struct passed to tcsetattr()
	// must have been initialized with a call to tcgetattr() overwise behaviour
	// is undefined
	if(tcgetattr(serial_port, &tty) != 0) {
	    printf("ERROR - SerialPosix::SerialPosix()  %s: error tcgetattr %i from tcgetattr %s\n", mDeviceName.c_str(),errno, strerror(errno));
	}
	//
	// set Control Modes
	//
	tty.c_cflag |= mParityEnable; // parity enable
	tty.c_cflag |= mParityType; // parity type
	tty.c_cflag |= mStopBits; // num. stop bits
	tty.c_cflag |= mCsNumber; // number of bits per byte
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	//
	// set Local Modes
	//
	if(mCanonicalMode)
		tty.c_lflag |= ICANON; // enable canonical mode
	else
		tty.c_lflag &= ~ICANON; // disable canonical mode
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	//
	// set Input Modes Modes
	//
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	//
	// set Output Modes
	//
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	//
	// set VMIN and VTIME
	//
	if(mBlocking) {
		// read() always wait for 1 byte (exactly how many is determined by VMIN), so read() could block indefinitely
		tty.c_cc[VTIME] = 0;
		tty.c_cc[VMIN] = 1;
	}
	else {
		// No blocking, return immediately with what is available
		tty.c_cc[VTIME] = 0;
		tty.c_cc[VMIN] = 0;
	}
	//
	// set I/O baud rate
	//
	cfsetispeed(&tty, mBaudRate);
	cfsetospeed(&tty, mBaudRate);
	//
	// Save tty settings, also checking for error
	//
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("ERROR SerialPosix::create %s ---Error %i from tcsetattr: %s\n", mDeviceName.c_str(), errno, strerror(errno));
		return false;
	  }
	return true;
}


bool SerialPosix::check_parameters(uint32_t baudRate,uint32_t parityEnable,
								   uint32_t parityType,uint32_t stopBits,uint32_t csNumber) {
	//
	// check baud rate
	//
	switch(baudRate) {
	case B0:
	case B50:
	case B75:
	case B110:
	case B134:
	case B150:
	case B200:
	case B300:
	case B600:
	case B1200:
	case B1800:
	case B2400:
	case B4800:
	case B9600:
	case B19200:
	case B38400:
	case B57600:
	case B115200:
	case B230400:
	case B460800:
	case B500000:
	case B576000:
	case B921600:
	case B1000000:
	case B1152000:
	case B1500000:
	case B2000000:
		break;
	default:
		printf("ERROR - SerialPosix::check_parameters wrong baud rate %" PRIu32 "\n",baudRate);
		return false;
	}
	//
	// check parity enable
	//
	switch(parityEnable) {
	case 0:
	case PARENB:
		break;
	default:
		printf("ERROR - SerialPosix::check_parameters wrong parity enable %" PRIu32 "\n",parityEnable);
		return false;
	}
	//
	// check parity type
	//
	switch(parityType) {
	case 0:
	case PARODD:
		break;
	default:
		printf("ERROR - SerialPosix::check_parameters wrong parity enable %" PRIu32 "\n",parityType);
		return false;
	}
	//
	// check num. stop bits
	//
	switch(stopBits) {
	case 0:
	case CSTOPB:
		break;
	default:
		printf("ERROR - SerialPosix::check_parameters wrong num. stop bits %" PRIu32 "\n",stopBits);
		return false;
	}
	//
	// check number of bits per byte
	//
	switch(csNumber) {
	case CS5:
	case CS6:
	case CS7:
	case CS8:
		break;
	default:
		printf("ERROR - SerialPosix::check_parameters wrong number of bits per byte %" PRIu32 "\n",csNumber);
		return false;
	}

	return true;
}
