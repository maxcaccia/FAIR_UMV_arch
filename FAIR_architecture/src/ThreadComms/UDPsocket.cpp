//---------------------------------------------------------------------------
//
// UdpSocket.cpp
//
// 	Note: bind_udp produces not blocking sockets
//
//---------------------------------------------------------------------------

#include "UDPsocket.h"

//---------------------------------------------------------------------------
//
// UdpSocket::UdpSocket(void)
//
//---------------------------------------------------------------------------
UdpSocket::UdpSocket(void)
{
	//
	// default UDP socket is set to any address
	//
	mSockId=0;
	mLocalSockAddr.sin_family=AF_INET;
	mLocalSockAddr.sin_addr.s_addr=inet_addr(ANY_ADDR);
	mLocalSockAddr.sin_port=htons(DEFAULT_PORT);

	if((mSockId=socket(AF_INET,SOCK_DGRAM,0))==-1) {
		printf("UdpSocket::open - socket call error (%d)\n",errno);
	}
}


//---------------------------------------------------------------------------
//
// UdpSocket::~UdpSocket(void)
//
//---------------------------------------------------------------------------
UdpSocket::~UdpSocket(void) {
	if(mSockId!=0)
		close_udp();
}

//---------------------------------------------------------------------------
//
// bool UdpSocket::bind_udp(int16_t localUdpPort)
//
//---------------------------------------------------------------------------
bool UdpSocket::bind_udp(uint16_t localUdpPort) {
	mLocalSockAddr.sin_port=htons(localUdpPort);
	if(bind(mSockId,(struct sockaddr*)&mLocalSockAddr,sizeof(struct sockaddr_in))==-1) {
		printf("UDPsocket::bind_udp - bind call error (%s)\n",strerror(errno));
		if(!close_udp())
			mSockId=0;
		return false;
	}
	disable_blocking();
	return true;
}


//---------------------------------------------------------------------------
//
// bool UdpSocket::bind_udp(char* localIpAddress,int16_t localUdpPort)
//
//---------------------------------------------------------------------------
bool UdpSocket::bind_udp(char* localIpAddress,uint16_t localUdpPort) {
	mLocalSockAddr.sin_addr.s_addr=inet_addr(localIpAddress);
	mLocalSockAddr.sin_port=htons(localUdpPort);
	if(bind(mSockId,(struct sockaddr*)&mLocalSockAddr,sizeof(struct sockaddr_in))==-1) {
		printf("UdpSocket::bind_udp - bind call error (%s)\n",strerror(errno));
		if(!close_udp())
			mSockId=0;
		return false;
	}
	disable_blocking();
	return true;
}

//
// bool UdpSocket::bind_udp(string localIpAddress,int16_t localUdpPort)
//
//---------------------------------------------------------------------------
bool UdpSocket::bind_udp(string localIpAddress,uint16_t localUdpPort) {
	char localAddress[64];
	strcpy(localAddress,localIpAddress.c_str());
	mLocalSockAddr.sin_addr.s_addr=inet_addr(localAddress);
	mLocalSockAddr.sin_port=htons(localUdpPort);
	if(bind(mSockId,(struct sockaddr*)&mLocalSockAddr,sizeof(struct sockaddr_in))==-1) {
		printf("UdpSocket::bind_udp - bind call error (%s)\n",strerror(errno));
		if(!close_udp())
			mSockId=0;
		return false;
	}
	disable_blocking();


	return true;
}


//---------------------------------------------------------------------------
//
// int UdpSocket::close_udp(void)
//
//---------------------------------------------------------------------------
bool UdpSocket::close_udp(void) {
	if(mSockId==0)
		return false;
	if(close(mSockId)<0) {
		printf("UdpSocket::close - close call error\n");
		return false;
    }
	mSockId=0;
	return true;
}


//---------------------------------------------------------------------------
//
// ssize_t UdpSocket::recvfrom(uint8_t* buf,int bufLen,int flags,char* peerIpAddress,uint16_t &peerUdpPort))
//
//---------------------------------------------------------------------------
ssize_t UdpSocket::recvfrom(uint8_t *buf,int bufLen,int flags,char* peerIpAddress,uint16_t &peerUdpPort) {
	struct sockaddr_in peer_addr;
	socklen_t peer_addr_len;
	peer_addr_len=sizeof(peer_addr);
	ssize_t n;
	n=recvfrom(buf, bufLen, flags,(struct sockaddr *) &peer_addr, &peer_addr_len);
//	n=recvfrom(buf, bufLen, flags,(struct sockaddr_in *) &peer_addr, &peer_addr_len);
	if(n!=-1) {
		strcpy(peerIpAddress,inet_ntoa(peer_addr.sin_addr));
		peerUdpPort=ntohs(peer_addr.sin_port);
	}
	return n;
}



ssize_t UdpSocket::sendto(const uint8_t *buf,int bufLen,int flags,char* peerIpAddress,uint16_t peerUdpPort) {
	if(mSockId==0) {
		printf("UdpSocket::sendto null socket id\n");
		return(-1);
	}
	struct sockaddr_in peer_addr;
//printf("UdpSocket::sendto step1 peerIpAddress %s peerUdpPort %" PRIu16 "\r\n",peerIpAddress,peerUdpPort);fflush(stdout);
	peer_addr.sin_family=AF_INET;
	peer_addr.sin_addr.s_addr=inet_addr(peerIpAddress);
	peer_addr.sin_port=htons(peerUdpPort);
//printf("UdpSocket::sendto step2 peerIpAddress %s peerUdpPort %" PRIu16 "\r\n",peerIpAddress,peer_addr.sin_port);fflush(stdout);
	return ::sendto(mSockId,buf,bufLen,flags,(struct sockaddr*)&peer_addr,sizeof(struct sockaddr_in));
//	return sendto(buf,bufLen,flags,(struct sockaddr*) &peer_addr,sizeof(struct sockaddr_in));

}

ssize_t UdpSocket::sendto(const uint8_t *buf,int bufLen,int flags,struct sockaddr_in addr) {
	if(mSockId==0) {
		printf("UdpSocket::sendto null socket id\n");
		return(-1);
	}
//printf("UdpSocket::sendto step2 peerIpAddress %s peerUdpPort %" PRIu16 "\r\n",peerIpAddress,peer_addr.sin_port);fflush(stdout);
	return ::sendto(mSockId,buf,bufLen,flags,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
//	return sendto(buf,bufLen,flags,(struct sockaddr*) &peer_addr,sizeof(struct sockaddr_in));

}


//---------------------------------------------------------------------------
//
// ssize_t UdpSocket::recvfrom(uint8_t* buf,int bufLen,int flags,struct sockaddr* from,int* pFromLen)
//
//---------------------------------------------------------------------------
ssize_t UdpSocket::recvfrom(uint8_t* buf,int bufLen,int flags,struct sockaddr* fromAddr,socklen_t* addrLength) {
//ssize_t UdpSocket::recvfrom(uint8_t* buf,int bufLen,int flags,struct sockaddr_in* fromAddr,socklen_t* addrLength) {
	int n;
    if(mSockId==0) {
		printf("UdpSocket::recvfrom null socket id\n");
		return(-1);
	}
    n=::recvfrom(mSockId,buf,bufLen,flags,(struct sockaddr*)fromAddr,addrLength);
//    socklen_t addrlen=sizeof(struct sockaddr_in);
//    n=::recvfrom(mSockId,buf,bufLen,flags,(struct sockaddr*)fromAddr,&addrlen);

//    struct sockaddr_in * src_addr=(struct sockaddr_in *)fromAddr;
//    if(n>0) {
//    	printf("UdpSocket::recvfrom address %s port %d message %s\n",inet_ntoa(src_addr->sin_addr),ntohs(src_addr->sin_port),(char*)buf);
//    	fflush(stdout);
//    }

	return n;
}

//---------------------------------------------------------------------------
//
// ssize_t UdpSocket::sendto(const uint8_t* buf,int bufLen,int flags,struct sockaddr* to,int tolen)
//
//---------------------------------------------------------------------------
ssize_t UdpSocket::sendto(const uint8_t* buf,int bufLen,int flags,struct sockaddr* toAddr,socklen_t addrLength) {
	if(mSockId==0) {
		printf("UdpSocket::sendto null socket id\n");
		return(-1);
	}
	return(::sendto(mSockId,buf,bufLen,flags,toAddr,addrLength));
}


//---------------------------------------------------------------------------
//
// ssize_t UdpSocket::recv(uint8_t* buf,int bufLen,int flags)
//
//---------------------------------------------------------------------------
ssize_t UdpSocket::recv(uint8_t* buf,int bufLen,int flags) {
	if(mSockId==0) {
		printf("UdpSocket::recv null socket id\n");
		return(-1);
	}
	int tmpLength;
	struct sockaddr_in tmpSockAddr;
	tmpLength=sizeof(struct sockaddr_in);
	return(::recvfrom(mSockId,buf,bufLen,flags,(struct sockaddr*)&tmpSockAddr,(socklen_t*)&tmpLength));
}


//
// ssize_t UdpSocket::recv(uint8_t* buf,int bufLen,int flags,struct sockaddr_in &addr)
//
//---------------------------------------------------------------------------
ssize_t UdpSocket::recv(uint8_t* buf,int bufLen,int flags,struct sockaddr_in &addr) {
	if(mSockId==0) {
		printf("UdpSocket::recv null socket id\n");
		return(-1);
	}
	int tmpLength;
	//struct sockaddr_in tmpSockAddr;
	tmpLength=sizeof(struct sockaddr_in);
	return(::recvfrom(mSockId,buf,bufLen,flags,(struct sockaddr*)&addr,(socklen_t*)&tmpLength));
}



//---------------------------------------------------------------------------
//
// ssize_t UdpSocket::send(const uint8_t* buf,int bufLen,int flags)
//
//---------------------------------------------------------------------------
ssize_t UdpSocket::send(const uint8_t* buf,int bufLen,int flags) {
    if(mSockId==0) {
		printf("UdpSocket::send null socket id\n");
		return(-1);
	}
    mPeerSockAddr.lock();
    if(mPeerSockAddr.n==0) {
//		printf("UdpSocket::send no peer sockets id\n");
    	mPeerSockAddr.unlock();
		return(-1);
	}
    ssize_t val,value; bool txOk=true;
    for(int i=0;i<mPeerSockAddr.n;i++) {
    	val = ::sendto(mSockId,buf,bufLen,flags,(struct sockaddr*)&mPeerSockAddr.sockAddr[i],sizeof(struct sockaddr_in));
    	if(val == -1)
    		txOk=false;
    	else
    		value=val;
    }
    mPeerSockAddr.unlock();
    if(!txOk)
    	return -1;
    else
    	return value;
}


ssize_t UdpSocket::select_send(const uint8_t* buf,int bufLen,int flags,int peerId) {
    if(mSockId==0) {
		printf("UdpSocket::select_send null socket id\n");
		return(-1);
	}
    mPeerSockAddr.lock();
    if(mPeerSockAddr.n==0) {
		printf("UdpSocket::select_send no peer sockets id\n");
		mPeerSockAddr.unlock();
		return(-1);
	}
    if(peerId>=mPeerSockAddr.n) {
		printf("UdpSocket::select_send peerId %d invalid id\n",peerId);
		mPeerSockAddr.unlock();
		return(-1);
    }
    ssize_t value;
//printf("UdpSocket::select_send %s %d\n",inet_ntoa(mPeerSockAddr.sockAddr[peerId].sin_addr),ntohs(mPeerSockAddr.sockAddr[peerId].sin_port));fflush(stdout);
//printf("UdpSocket::select_send %s",(char*)buf);
    value = ::sendto(mSockId,buf,bufLen,flags,(struct sockaddr*)&mPeerSockAddr.sockAddr[peerId],sizeof(struct sockaddr_in));
    mPeerSockAddr.unlock();
    return value;
}

//---------------------------------------------------------------------------
//
// int UdpSocket::enable_blocking(void)
//
//---------------------------------------------------------------------------
bool UdpSocket::enable_blocking(struct timeval *tm) {
    if(mSockId==0) {
		printf("UdpSocket::enable_blocking null socket id\n");
		return false;
	}
    int flags;
//    if(tm==NULL) {
    	if((flags=fcntl(mSockId,F_GETFL,0))==-1) {
    		printf("UdpSocket::enable_blocking - fcntl call error (%d)\n",errno);
    	    return false;
    	}
    	if(fcntl(mSockId,F_SETFL,flags&(~O_NONBLOCK))==-1) {
    		printf("UdpSocket::enable_blocking - fcntl call error (%d)\n",errno);
    		return false;
    	    }
//    }
//    else {
	if(tm!=NULL) {
		if(setsockopt(mSockId,SOL_SOCKET,SO_RCVTIMEO,(char*)tm,sizeof(struct timeval))==-1) {
			printf("UdpSocket::enable_blocking - setsockopt call error (%d)\n",errno);
		return false;
	}
printf("UdpSocket::enable_blocking %d\n",mSockId);
    }
    return true;
}


//---------------------------------------------------------------------------
//
// int UdpSocket::disable_blocking(void)
//
//---------------------------------------------------------------------------
bool UdpSocket::disable_blocking(void)
{
    if(mSockId==0) {
		printf("UdpSocket::disable_blocking null socket id\n");
		return false;
	}
    int flags;
	if((flags=fcntl(mSockId,F_GETFL,0))==-1) {
		printf("UdpSocket::disable_blocking - fcntl call error (%d)\n",errno);
	    return false;
	}
	if(fcntl(mSockId,F_SETFL,flags|O_NONBLOCK)==-1) {
		printf("UdpSocket::disable_blocking - fcntl call error (%d)\n",errno);
		return false;
	    }
	return true;
}


////---------------------------------------------------------------------------
////
//// int Datagram::enable_broadcast(void)
////
////---------------------------------------------------------------------------
//int Datagram::enable_broadcast(void)
//{
//    if(sockId==0) return(-1);
//
//int on=1;       // activation flag for setsockopt
//
// if(setsockopt(sockId,SOL_SOCKET,SO_BROADCAST,(char *)&on,sizeof(on))==-1)
//    {
//    printf("Datagram::enable_broadcast - setsockopt call error (SO_BROADCAST option) (%d)\n",errno);
//    return(-1);
//    }
//
//// dprintf("Datagram::enable_broadcast - enabled broadcast mode\n");
//
// return(0);
//}
//
//
////---------------------------------------------------------------------------
////
//// int Datagram::disable_broadcast(void)
////
////---------------------------------------------------------------------------
//int Datagram::disable_broadcast(void)
//{
//    if(sockId==0) return(-1);
//
//int on=0;       // disactivation flag for setsockopt
//
// if(setsockopt(sockId,SOL_SOCKET,SO_BROADCAST,(char *)&on,sizeof(on))==-1)
//    {
//    printf("Datagram::disable_broadcast - setsockopt call error (SO_BROADCAST option) (%d)\n",errno);
//    return(-1);
//    }
//
//// dprintf("Datagram::disable_broadcast - enabled broadcast mode\n");
//
// return(0);
//}
