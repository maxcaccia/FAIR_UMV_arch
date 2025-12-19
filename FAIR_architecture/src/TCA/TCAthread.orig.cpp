/*
 * TCAthread.cpp
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#include "TCAthread.h"

TCAthread::TCAthread(void): CntrlSimThread() {
	//printf("TCAthread::TCAthread begin %s %d %d\n",mName.c_str(),mPolicy,mPriority);fflush(stdout);
	mTCA=NULL; // mTCA is instantiated in the child class <Robot>TCAthread
//	printf("TCAthread::TCAthread end %s %d %d\n",mName.c_str(),mPolicy,mPriority);fflush(stdout);
}

TCAthread::~TCAthread() {
	// TODO Auto-generated destructor stub
	if(mTCA!=NULL) {
		delete mTCA;
		mTCA=NULL;
	}
}

void TCAthread::make_thread()
{
//    printf("TCAthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&TCAthread::execute,this);
}

bool TCAthread::init_configuration() {
//	printf("TCAthread::init_configuration %s\n",mName.c_str());fflush(stdout);

	mForceMomentRef.set_name("ref_"+mName);
	mForceMomentHat.set_name("est_"+mName);

	mTCA->set_input_output(&mForceMomentRef,&mForceMomentHat);

	mTCAstate.set_name("state_"+mName);
	mTCA->set_state(&mTCAstate);
//	mTCA->reset();

	return true;
}

void TCAthread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- TCAthread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="TCA") {
		if(mTcaCmd.decode(cmd, strCmd)) {
			if(strCmd=="ENABLE") {
				mTCAstate.mEnable=true;
				mTCA->set_manual_default();
			}
			else if(strCmd=="DISABLE") {
				mTCAstate.mEnable=false;
				mTCA->reset();
			}
			else if((strCmd=="SET_2DOF")||(strCmd=="SET_3DOF")||(strCmd=="SET_4DOF")||(strCmd=="SET_6DOF"))
				process_set_n_dof();
			else if(strCmd=="SET_MODE") {
				process_set_mode();
			}
			else if(strCmd=="SET_FUNCTION") {
				process_set_function();
			}
			else if(strCmd=="SET_CONFIG") {
				process_set_config();
			}
			else  {
//				printf("TCAthread::decode ERROR read - TCA cmd is %s\n",strCmd.c_str());
//				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="TCA")
	else  {
//		printf("TCAthread::process_command ERROR read - type is %s\n",strType.c_str());
//		fflush(stdout);
		return ;
	}

}

void TCAthread::build_data(void) {
	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	packet.n=mForceMomentRef.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
	printf("TCAthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mForceMomentHat.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
	printf("TCAthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

	packet.n=mTCAstate.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
	printf("TCAthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
}

