/*
 * GNSSthread.cpp
 *
 *  Created on: Feb 13, 2024
 *      Author: mc
 */

//***** insert your code here ************************************

#include "GNSSthread.h"

GNSSthread::GNSSthread(void): DriverThread() {
//	printf("GNSSthread::GNSSthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);

	mGnssPositionData = new GnssPositionData(mName);
	mGnssVelocityData = new GnssVelocityData(mName);
}

GNSSthread::~GNSSthread() {
	// TODO Auto-generated destructor stub
}

void GNSSthread::make_thread()
{
	printf("GNSSthread::make_thread %s\n",mName.c_str());fflush(stdout);
	mThread = new thread(&GNSSthread::execute,this);
//	mThread = new thread(&execute,this);
	printf("GNSSthread::make_thread %s done\n",mName.c_str());fflush(stdout);

}


void GNSSthread::end(void)
{
	printf("GNSSthread::end %s\n",mName.c_str());

	DriverThread::end();
	if(mGnssPositionData!=NULL) {
		delete mGnssPositionData;
		mGnssPositionData=NULL;
	}
	if(mGnssVelocityData!=NULL) {
		delete mGnssVelocityData;
		mGnssVelocityData=NULL;
	}}

void GNSSthread::execute_thread_semantics(void) {
//	printf("GNSSthread::execute_thread_semantics() %s\n",mName.c_str());

	//
	// get data from data parser
	//
	((NMEAdataParser*)mDataParser)->get_data(*mGnssPositionData,*mGnssVelocityData);
	// assign thread name to data
	mGnssPositionData->set_name(mName);
	mGnssVelocityData->set_name(mName);
	//
	//
	// execute semantics
	//

	// nothing to do, in this case

}


void GNSSthread::build_data(void) {
	//
	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	packet.n=mGnssPositionData->sprint((char*)packet.data);
	mDataSendBuffer.push(packet);
printf("void GNSSthread::build_data %s\n",packet.data);
	packet.n=mGnssVelocityData->sprint((char*)packet.data);
	mDataSendBuffer.push(packet);
}


