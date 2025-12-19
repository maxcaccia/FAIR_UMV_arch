/*
 * DAGPSthread.cpp
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

//***** insert your code here ************************************

#include "DAGPSthread.h"

DAGPSthread::DAGPSthread(void): DriverThread() {
//***** end insert your code *************************************
//	printf("MIPthread::MIPthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);

	//***** insert your code here ************************************
	//
	// instantiate device data
	//
	mGnssPositionData = new GnssPositionData(mName);
	mGnssVelocityData = new GnssVelocityData(mName);
	mTrueHeadingData = new TrueHeadingData(mName);
	//***** end insert your code *************************************

}

//***** insert your code here ************************************
DAGPSthread::~DAGPSthread() {
//***** end insert your code *************************************
	// TODO Auto-generated destructor stub
}

//***** insert your code here ************************************
void DAGPSthread::make_thread()
{
	printf("DAGPSthread::make_thread %s\n",mName.c_str());fflush(stdout);
	mThread = new thread(&DAGPSthread::execute,this);
//	mThread = new thread(&execute,this);
	printf("DAGPSthread::make_thread %s done\n",mName.c_str());fflush(stdout);

}
//***** end insert your code *************************************


//***** insert your code here ************************************
//
void DAGPSthread::end(void)
{
	printf("DAGPSthread::end %s\n",mName.c_str());
//***** end insert your code *************************************

	DriverThread::end();
	//***** insert your code here ************************************
	if(mGnssPositionData!=NULL) {
		delete mGnssPositionData;
		mGnssPositionData=NULL;
	}
	if(mGnssVelocityData!=NULL) {
		delete mGnssVelocityData;
		mGnssVelocityData=NULL;
	}
	if(mTrueHeadingData!=NULL) {
		delete mTrueHeadingData;
		mTrueHeadingData=NULL;
	}
	//***** end insert your code *************************************
}


//***** insert your code here ************************************
void DAGPSthread::execute_thread_semantics(void) {
//***** end insert your code *************************************
//	printf("DAGPSthread::execute_thread_semantics() %s\n",mName.c_str());

	//***** insert your code here ************************************

	//
	// get data from data parser
	//
	((NMEAdataParser*)mDataParser)->get_data((GnssPositionData&)*mGnssPositionData,(GnssVelocityData&)*mGnssVelocityData);
	((NMEAdataParser*)mDataParser)->get_data((TrueHeadingData&)*mTrueHeadingData);
	//***** end insert your code *************************************

	//***** insert your code here ************************************
	//
	//
	// execute semantics
	//

	// nothing to do, in this case

	//***** end insert your code *************************************

	//***** insert your code here ************************************
	//
	//
	// append output to mDataSendBuffer
	//
	Packet packet1,packet2,packet3;
	packet1.n=mGnssPositionData->sprint((char*)packet1.data);
	mDataSendBuffer.push(packet1);
	packet2.n=mGnssVelocityData->sprint((char*)packet2.data);
	mDataSendBuffer.push(packet2);
	packet3.n=mTrueHeadingData->sprint((char*)packet3.data);
	mDataSendBuffer.push(packet3);
//printf("void DAGPSthread::execute_thread_semantics %s %s1\n",str1.c_str(),str2.c_str());
}




