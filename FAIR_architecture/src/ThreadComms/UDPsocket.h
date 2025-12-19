//---------------------------------------------------------------------------
//
// UDPsocket.h
//
// 	Note: bind_udp produces not blocking sockets
//
//---------------------------------------------------------------------------

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <inttypes.h>
#include <string>
using namespace std;

#include <errno.h>
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
//#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>


#include "SocketDefine.h"

struct SockAddrArrayMutex {
	struct sockaddr_in sockAddr[MAX_SOCK_ADDR];
	int n=0;
	pthread_mutex_t mux=PTHREAD_MUTEX_INITIALIZER;

	void lock(void) { pthread_mutex_lock(&mux); }
	void unlock(void) { pthread_mutex_unlock(&mux); }

	int is_in(uint32_t addr,uint16_t port)
	// return:	-1 if (addr,port) is not in
	//			index if (addr,port) is in
	{
		for(int i=0;i<n;i++)
			if((sockAddr[i].sin_addr.s_addr==addr)&&(sockAddr[i].sin_port==htons(port)))
				return i;
		return -1;
	}

	bool add(uint32_t addr,uint16_t port) {
		// if (addr,port) is already in then returns true while doing nothing
		if(n==(MAX_SOCK_ADDR-1))
			return false;
		int m;
		if((m=is_in(addr,port))==-1) {
			sockAddr[n].sin_family=AF_INET;
			sockAddr[n].sin_addr.s_addr=addr;
			sockAddr[n].sin_port=htons(port);
			n++;
		}
		else {
			printf("SockAddrArrayMutex::add address %s port %" PRIu16 " is already registered\n",
					inet_ntoa(sockAddr[m].sin_addr),port); fflush(stdout);
		}
		return true;
	}

	bool remove(uint32_t addr,uint16_t port) {
		int index;
		index=is_in(addr,port);
		if(index==-1)
			return false;
		for(int i=index;i<(n-1);i++)
		{
			sockAddr[i].sin_family=sockAddr[i+1].sin_family;
			sockAddr[i].sin_addr.s_addr=sockAddr[i+1].sin_addr.s_addr;
			sockAddr[i].sin_port=sockAddr[i+1].sin_port;
		}
		n--;
		return true;
	}
};

class UdpSocket {
public:
	UdpSocket(void);
	~UdpSocket(void);

	bool bind_udp(uint16_t localUdpPort);
	bool bind_udp(char* localIpAddress,uint16_t localUdpPort);
	bool bind_udp(string localIpAddress,uint16_t localUdpPort);

	bool close_udp(void);

	ssize_t recvfrom(uint8_t *buf,int bufLen,int flags,char* peerIpAddress,uint16_t &peerUdpPort);
    ssize_t sendto(const uint8_t *buf,int bufLen,int flags,char* peerIpAddress,uint16_t peerUdpPort);
    ssize_t sendto(const uint8_t *buf,int bufLen,int flags,struct sockaddr_in addr);

    ssize_t recv(uint8_t *buf,int bufLen,int flags);
    ssize_t recv(uint8_t* buf,int bufLen,int flags,struct sockaddr_in &addr);
    ssize_t send(const uint8_t *buf,int bufLen,int flags);
    ssize_t select_send(const uint8_t *buf,int bufLen,int flags,int peerId);

    bool enable_blocking(struct timeval *tm=NULL);
    bool disable_blocking(void);

    bool add_peer(char *addr,uint16_t port) { bool flag; mPeerSockAddr.lock(); flag=mPeerSockAddr.add(inet_addr(addr),port); mPeerSockAddr.unlock(); return flag; }
    bool remove_peer(char *addr,uint16_t port) { bool flag; mPeerSockAddr.lock(); flag=mPeerSockAddr.remove(inet_addr(addr),port); mPeerSockAddr.unlock(); return flag; }

private:
	ssize_t recvfrom(uint8_t *buf,int bufLen,int flags,struct sockaddr *fromAddr,socklen_t *addrLength);
//	ssize_t recvfrom(uint8_t *buf,int bufLen,int flags,struct sockaddr_in *fromAddr,socklen_t *addrLength);
    ssize_t sendto(const uint8_t *buf,int bufLen,int flags,struct sockaddr  *toAddr,socklen_t addrLength);

//    int enable_broadcast(void);
//    int disable_broadcast(void);
private:
    int mSockId;
    SockAddrArrayMutex mPeerSockAddr;
    struct sockaddr_in mLocalSockAddr;
};


#endif // UDPSOCKET_H

