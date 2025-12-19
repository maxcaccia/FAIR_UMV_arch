/*
 * RingBuffer.h
 *
 *  Created on: Nov 3, 2023
 *      Author: mc
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <string>
using namespace std;

#define DEFAULT_BUFFER_SIZE 2048

template <class T> class RingBuffer {
	string mName;
	int mHead;
	int mTail;
	int mSize;
	T mBuffer[DEFAULT_BUFFER_SIZE];

	int next(int value) { return((value+1)%mSize); }
public:
	RingBuffer(void) {
//		printf("RingBuffer::RingBuffer 1\n");fflush(stdout);
		mName="";
		mSize=DEFAULT_BUFFER_SIZE;
		mHead=mTail=0;
//		printf("RingBuffer::RingBuffer 2\n");fflush(stdout);
	}

	RingBuffer(string name) {
		mName=name;
		mSize=DEFAULT_BUFFER_SIZE;
		mHead=mTail=0;
	}

	~RingBuffer(void) {}

	void set_name(string name) {
		mName=name;
	}

	string get_name(void) { return mName; }

	bool push(T item) {
//printf("RingBuffer %s push\n",mName.c_str());fflush(stdout);
		mHead=next(mHead);
		if(mHead==mTail) {
			printf("ERROR %s RingBuffer::push(...) is full\n",mName.c_str());
			return false;
		}
		mBuffer[mHead]=item;
		return true;
	}

	bool pop(T &item) {
		if(mHead==mTail) {
//			printf("ERROR %s RingBufferMutex::pop(...) is empty\n",mName.c_str());
			return false;
		}
		mTail=next(mTail);
		item=mBuffer[mTail];
		return true;
	}

	void reset(void) { mHead=mTail=0;}
};

#endif /* RINGBUFFER_H_ */
