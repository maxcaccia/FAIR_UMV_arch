/*
 * LoggerThread.cpp
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#include "LoggerThread.h"

LoggerThread::LoggerThread(void): CntrlSimThread() {
//	printf("LoggerThread::LoggerThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mLogFile=NULL;
}

LoggerThread::~LoggerThread() {
	// TODO Auto-generated destructor stub
	if(mLogFile!=NULL) {
		fclose(mLogFile);
		mLogFile=NULL;
	}
}

void LoggerThread::make_thread()
{
//    printf("LoggerThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&LoggerThread::execute,this);
}


void LoggerThread::process_command(char *cmd) {
	printf("******************** %s ---- LoggerThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
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

void LoggerThread::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;
	if(dataStrType=="AHRS")
		mAhrsData.sread(data);
	else if(dataStrType=="GPS")
		mGpsData.sread(data);
	else if(dataStrType=="TRUE_HEADING")
		mTrueHeadingData.sread(data);
	else if(dataStrType=="KIN_DYN")
		mKinHat.sread(data);
	else if(dataStrType=="FORCE_TORQUE")
		mForceTorqueRef.sread(data);
	else if(dataStrType=="SWAMP_TLM")
		mSwampTlm.sread(data);

}

void LoggerThread::execute_thread_semantics(void) {
	//printf("LoggerThread::execute_thread_semantics() %s\n",mName.c_str());
	log_data();
}

void LoggerThread::log_data(void) {
	//printf("LoggerThread::log_data() %s\n",mName.c_str());
	char packet[2560];
	char strAhrs[256];
	char strGps[256];
	char strTrueHeading[256];
	char strKinDynHat[512];
	char strSwampTlm[1024];
	mAhrsData.sprint_csv(strAhrs);
	mGpsData.sprint_csv(strGps);
	mTrueHeadingData.sprint_csv(strTrueHeading);
	mKinHat.sprint_csv(strKinDynHat);
	mSwampTlm.sprint_csv(strSwampTlm);
sprintf(packet,"%s%s%s%s",strAhrs,strGps,strTrueHeading,strSwampTlm);
//printf("LoggerThread::log_data : %s\r\n",packet);fflush(stdout);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
}

void LoggerThread::log_headers(void) {
	char packet[4096];
	char strAhrs[512];
	char strGps[512];
	char strTrueHeading[512];
	char strSwampTlm[1024];
	// write log names
	mAhrsData.sprint_csv_log_name(strAhrs);
	mGpsData.sprint_csv_log_name(strGps);
	mTrueHeadingData.sprint_csv_log_name(strTrueHeading);
	mSwampTlm.sprint_csv_log_name(strSwampTlm);
	sprintf(packet,"%s%s%s%s",strAhrs,strGps,strTrueHeading,strSwampTlm);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
	// write long names
	mAhrsData.sprint_csv_long_name(strAhrs);
	mGpsData.sprint_csv_long_name(strGps);
	mTrueHeadingData.sprint_csv_long_name(strTrueHeading);
	mSwampTlm.sprint_csv_long_name(strSwampTlm);
	sprintf(packet,"%s%s%s%s",strAhrs,strGps,strTrueHeading,strSwampTlm);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
}




