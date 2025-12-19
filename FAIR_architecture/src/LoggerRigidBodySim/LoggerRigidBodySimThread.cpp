/*
 * LoggerRigidBodySimThread.cpp
 *
 *  Created on: Dec 5, 2023
 *      Author: mc
 */

#include "LoggerRigidBodySimThread.h"

LoggerRigidBodySimThread::LoggerRigidBodySimThread(void): CntrlSimThread() {
//	printf("LoggerRigidBodySimThread::LoggerRigidBodySimThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mLogFile=NULL;
}

LoggerRigidBodySimThread::~LoggerRigidBodySimThread() {
	// TODO Auto-generated destructor stub
	if(mLogFile!=NULL) {
		fclose(mLogFile);
		mLogFile=NULL;
	}
}

void LoggerRigidBodySimThread::make_thread()
{
//    printf("LoggerRigidBodySimThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&LoggerRigidBodySimThread::execute,this);
}


void LoggerRigidBodySimThread::process_command(char *cmd) {
	printf("******************** %s ---- LoggerRigidBodySimThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
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

void LoggerRigidBodySimThread::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
//printf("LoggerRigidBodySimThread::process_data %s\n",data);
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;

	if(dataStrType==LINEAR_POSITION_CODE)
		mPosition.sread(data);
	else if(dataStrType==ANGULAR_POSITION_CODE)
		mAttitude.sread(data);
	else if(dataStrType==LINEAR_VELOCITY_CODE)
		mLinearSpeed.sread(data);
	else if(dataStrType==ANGULAR_VELOCITY_CODE)
		mAngSpeed.sread(data);
	else if(dataStrType==FORCE_MOMENT_CODE)
		mForceMoment.sread(data);



}

void LoggerRigidBodySimThread::execute_thread_semantics(void) {
	//printf("LoggerRigidBodySimThread::execute_thread_semantics() %s\n",mName.c_str());
	log_data();
}

void LoggerRigidBodySimThread::log_data(void) {
	//printf("LoggerThread::log_data() %s\n",mName.c_str());
	char packet[2560];
	char strLinPos[512];
	char strLinVel[512];
	char strAngPos[512];
	char strAngVel[512];
	char strForceMoment[512];
	mPosition.sprint_csv(strLinPos);
	mAttitude.sprint_csv(strAngPos);
	mLinearSpeed.sprint_csv(strLinVel);
	mAngSpeed.sprint_csv(strAngVel);
	mForceMoment.sprint_csv(strForceMoment);
	sprintf(packet,"%s%s%s%s%s",strLinPos,strAngPos,strLinVel,strAngVel,strForceMoment);
//printf("LoggerRigidBodySimThread::log_data : %s\r\n",packet);fflush(stdout);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
}

void LoggerRigidBodySimThread::log_headers(void) {
	char packet[2560];
	char strLinPos[512];
	char strLinVel[512];
	char strAngPos[512];
	char strAngVel[512];
	char strForceMoment[512];
	// write log names
	mPosition.sprint_csv_log_name(strLinPos);
	mAttitude.sprint_csv_log_name(strAngPos);
	mLinearSpeed.sprint_csv_log_name(strLinVel);
	mAngSpeed.sprint_csv_log_name(strAngVel);
	mForceMoment.sprint_csv_log_name(strForceMoment);
	sprintf(packet,"%s%s%s%s%s",strLinPos,strAngPos,strLinVel,strAngVel,strForceMoment);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
	// write long names
	mPosition.sprint_csv_long_name(strLinPos);
	mAttitude.sprint_csv_long_name(strAngPos);
	mLinearSpeed.sprint_csv_long_name(strLinVel);
	mAngSpeed.sprint_csv_long_name(strAngVel);
	mForceMoment.sprint_csv_long_name(strForceMoment);
	sprintf(packet,"%s%s%s%s%s",strLinPos,strAngPos,strLinVel,strAngVel,strForceMoment);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}

}




