/*
 * LoggerTCAthread.cpp
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#include "LoggerTCAthread.h"

LoggerTCAthread::LoggerTCAthread(void): CntrlSimThread() {
//	printf("LoggerTCAthread::LoggerTCAthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mLogFile=NULL;
}

LoggerTCAthread::~LoggerTCAthread() {
	// TODO Auto-generated destructor stub
	if(mLogFile!=NULL) {
		fclose(mLogFile);
		mLogFile=NULL;
	}
}

void LoggerTCAthread::make_thread()
{
//    printf("LoggerTCAthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&LoggerTCAthread::execute,this);
}


void LoggerTCAthread::process_command(char *cmd) {
	printf("******************** %s ---- LoggerTCAthread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	time_t t;
	struct tm *tmp;
	char cmdType[64];
	char name[64];
	sscanf(cmd,"%s",cmdType);
	string cmdStrType=cmdType;
	if(cmdStrType=="start") {
		if(mLogFile!=NULL)
			fclose(mLogFile);
		sscanf(cmd,"%s %s",cmdType, name);
	    t = time(NULL);
	    tmp = gmtime(&t);
	    char fileName[256];
	    sprintf(fileName,"./DATA/%s_%d_%d.csv%c",name,(1900+tmp->tm_year)*10000 + (tmp->tm_mon+1)*100 + tmp->tm_mday,int(tmp->tm_hour*10000+tmp->tm_min*100+tmp->tm_sec),'\0');
	    mLogFile=fopen(fileName,"w"),
	    log_headers();
	}
	else if(cmdStrType=="stop") {
		if(mLogFile!=NULL)
			fclose(mLogFile);
		mLogFile=NULL;
	}

}

void LoggerTCAthread::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;

	SwampActuation swampActuation;
	ForceMoment forceMoment;

	if(dataStrType==SWAMP_ACTUATION_CODE) {
		swampActuation.sread(data);
		if(swampActuation.mName.substr(0,3)=="msr")
			mSwampActuationMsr=swampActuation;
		else if(swampActuation.mName.substr(0,3)=="ref")
			mSwampActuationRef=swampActuation;
		else {
			printf("LoggerTCAthread::process_data unexpected name %s\n",swampActuation.mName.c_str());
			fflush(stdout);
		}
	}
	else if(dataStrType==TCA_STATE_CODE)
		mTCAstate.sread(data);
	else if(dataStrType==FORCE_MOMENT_CODE) {
		forceMoment.sread(data);
		if(forceMoment.mName.substr(0,3)=="est")
			mForceMomentEst=forceMoment;
		else if(swampActuation.mName.substr(0,3)=="ref")
			mForceMomentRef=forceMoment;
		else {
			printf("LoggerTCAthread::process_data unexpected name %s\n",forceMoment.mName.c_str());
			fflush(stdout);
		}
	}

	printf("LoggerTCAthread::process_data %s",data);
}

void LoggerTCAthread::execute_thread_semantics(void) {
	//printf("LoggerTCAthread::execute_thread_semantics() %s\n",mName.c_str());
	log_data();
}

void LoggerTCAthread::log_data(void) {
	//printf("LoggerThread::log_data() %s\n",mName.c_str());
	char packet[2560];
	char strSwampActuationRef[512];
	char strSwampActuationMsr[512];
	char strTCAstate[512];
	char strForceMomentRef[512];
	char strForceMomentEst[512];

	mSwampActuationRef.sprint_csv(strSwampActuationRef);
	mSwampActuationMsr.sprint_csv(strSwampActuationMsr);
	mTCAstate.sprint_csv(strTCAstate);
	mForceMomentRef.sprint_csv(strForceMomentRef);
	mForceMomentEst.sprint_csv(strForceMomentEst);
	sprintf(packet,"%s%s%s%s%s",strTCAstate,strSwampActuationRef,strSwampActuationMsr,strForceMomentRef,strForceMomentEst);
//	sprintf(packet,"%s",strSwampActuation);
//printf("LoggerTCAthread::log_data : %s\r\n",packet);fflush(stdout);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
}

void LoggerTCAthread::log_headers(void) {
	char packet[2560];
	char strSwampActuationRef[512];
	char strSwampActuationMsr[512];
	char strTCAstate[512];
	char strForceMomentRef[512];
	char strForceMomentEst[512];

	// write log names
	mSwampActuationRef.sprint_csv_log_name(strSwampActuationRef);
	mSwampActuationMsr.sprint_csv_log_name(strSwampActuationMsr);
	mTCAstate.sprint_csv_log_name(strTCAstate);
	mForceMomentRef.sprint_csv_log_name(strForceMomentRef);
	mForceMomentEst.sprint_csv_log_name(strForceMomentEst);
	sprintf(packet,"%s%s%s%s%s",strTCAstate,strSwampActuationRef,strSwampActuationMsr,strForceMomentRef,strForceMomentEst);
//	sprintf(packet,"%s",strSwampActuation);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
	// write long names
	mSwampActuationRef.sprint_csv_long_name(strSwampActuationRef);
	mSwampActuationMsr.sprint_csv_long_name(strSwampActuationMsr);
	mTCAstate.sprint_csv_long_name(strTCAstate);
	mForceMomentRef.sprint_csv_long_name(strForceMomentRef);
	mForceMomentEst.sprint_csv_long_name(strForceMomentEst);
	sprintf(packet,"%s%s%s%s%s",strTCAstate,strSwampActuationRef,strSwampActuationMsr,strForceMomentRef,strForceMomentEst);

//	sprintf(packet,"%s",strSwampActuation);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}

}




