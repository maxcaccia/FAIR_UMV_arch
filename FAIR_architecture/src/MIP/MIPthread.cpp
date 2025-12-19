/*
 * MIPthread.cpp
 *
 *  Created on: Nov 10, 2023
 *      Author: mc
 */

//***** insert your code here ************************************
#include "MIPthread.h"

MIPthread::MIPthread(void): DriverThread() {
//***** end insert your code *************************************
//	printf("MIPthread::MIPthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);

	//***** insert your code here ************************************
	mAhrsData = new AhrsData(mName);
	//***** end insert your code *************************************

}

//***** insert your code here ************************************
MIPthread::~MIPthread() {
//***** end insert your code *************************************
	// TODO Auto-generated destructor stub
}

//***** insert your code here ************************************
void MIPthread::make_thread()
{
    printf("MIPthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&MIPthread::execute,this);
}
//***** end insert your code *************************************


//***** insert your code here ************************************
void MIPthread::end(void)
{
	printf("MIPthread::end %s\n",mName.c_str());
//***** end insert your code *************************************

	DriverThread::end();
	//***** insert your code here ************************************

	if(mAhrsData!=NULL) {
		delete mAhrsData;
		mAhrsData=NULL;
	}
	//***** end insert your code *************************************
}


//***** insert your code here ************************************
void MIPthread::execute_thread_semantics(void) {
//***** end insert your code *************************************
	printf("MIPthread::execute_thread_semantics() %s %lf\n",mName.c_str(),mTimeStamp);fflush(stdout);

	//***** insert your code here ************************************
	//
	// get data from data parser
	//
	((MIPdataParser*)mDataParser)->get_data(*mAhrsData);
	mAhrsData->set_name(mName);

	//***** end insert your code *************************************

	//***** insert your code here ************************************
	//
	//
	// execute semantics
	//

	// nothing to do, in this case

	//***** end insert your code *************************************

	//***** insert your code here ************************************

}

void MIPthread::build_data(void) {
	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	packet.n=mAhrsData->sprint((char*)packet.data);
	mDataSendBuffer.push(packet);
	printf("void MIPthread::build_data %s\n",packet.data);

//	mAhrsData->mValid=false;
}


