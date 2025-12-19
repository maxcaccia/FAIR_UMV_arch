/*
 * LoggerBlueBoatThread.cpp
 *
 *  Created on: May 9, 2025
 *      Author: mc
 */

#include "LoggerBlueBoatThread.h"

LoggerBlueBoatThread::LoggerBlueBoatThread(void): CntrlSimThread() {
//	printf("LoggerBlueBoatThread::LoggerRigidBodySimThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mLogFile=NULL;
}

LoggerBlueBoatThread::~LoggerBlueBoatThread() {
	// TODO Auto-generated destructor stub
	if(mLogFile!=NULL) {
		fclose(mLogFile);
		mLogFile=NULL;
	}
}

void LoggerBlueBoatThread::make_thread()
{
//    printf("LoggerBlueBoatThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&LoggerBlueBoatThread::execute,this);
}


void LoggerBlueBoatThread::process_command(char *cmd,ssize_t n) {
	printf("******************** %s ---- LoggerBlueBoatThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
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
printf("start log %s\n",fileName);fflush(stdout);
	    mLogFile=fopen(fileName,"w"),
	    log_headers();
	}
	else if(cmdStrType=="stop") {
		printf("stop log\n");fflush(stdout);

		if(mLogFile!=NULL)
			fclose(mLogFile);
		mLogFile=NULL;
	}

}

void LoggerBlueBoatThread::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
//printf("LoggerBlueBoatThread::process_data %s\n",data);
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;

	if(dataStrType==NORMALISED_RPM_CODE) {
		mNormRpm.sread(data);
		printf("LoggerBlueBoatThread::process_data %s\n",data);
	}
	else if(dataStrType==FORCE_MOMENT_CODE)
		mForceMoment.sread(data);
	else if(dataStrType==AHRS_CODE)
		mAhrsData.sread(data);
	else if(dataStrType==GNSS_POSITION_CODE)
		mGnssPosData.sread(data);
	else if(dataStrType==GNSS_VELOCITY_CODE)
		mGnssVelData.sread(data);
	else if(dataStrType==BATTERY_CODE)
		mBatteryData.sread(data);

}

void LoggerBlueBoatThread::execute_thread_semantics(void) {
	//printf("LoggerBlueBoatThread::execute_thread_semantics() %s\n",mName.c_str());
	log_data();
}

void LoggerBlueBoatThread::log_data(void) {
	//printf("LoggerThread::log_data() %s\n",mName.c_str());
	char packet[6144];
	char strNormRpm[512];
	char strForceMoment[512];
	char strAhrsData[512];
	char strGnssPositionData[512];
	char strGnssVelocityData[512];
	char strBatteryData[512];
	mNormRpm.sprint_csv(strNormRpm);
	mForceMoment.sprint_csv(strForceMoment);
	mAhrsData.sprint_csv(strAhrsData);
	mGnssPosData.sprint_csv(strGnssPositionData);
	mGnssVelData.sprint_csv(strGnssVelocityData);
	mBatteryData.sprint_csv(strBatteryData);
	sprintf(packet,"%s%s%s%s%s%s",strNormRpm,strForceMoment,
			strAhrsData,strGnssPositionData,strGnssVelocityData,strBatteryData);
//	sprintf(packet,"%s%s",strNormRpm,strForceMoment);
//printf("LoggerBlueBoatThread::log_data : %s\r\n",packet);fflush(stdout);
//	printf("LoggerBlueBoatThread::log_data : %s\r\n",strForceMoment);fflush(stdout);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
}

void LoggerBlueBoatThread::log_headers(void) {
	char packet[6144];
	char strNormRpm[512];
	char strForceMoment[512];
	char strAhrsData[512];
	char strGnssPositionData[512];
	char strGnssVelocityData[512];
	char strBatteryData[512];
	// write log names
	mNormRpm.sprint_csv_log_name(strNormRpm);
	mForceMoment.sprint_csv_log_name(strForceMoment);
	mAhrsData.sprint_csv_log_name(strAhrsData);
	mGnssPosData.sprint_csv_log_name(strGnssPositionData);
	mGnssVelData.sprint_csv_log_name(strGnssVelocityData);
	mBatteryData.sprint_csv_log_name(strBatteryData);
	sprintf(packet,"%s%s%s%s%s%s",strNormRpm,strForceMoment,
			strAhrsData,strGnssPositionData,strGnssVelocityData,strBatteryData);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
	printf("LoggerBlueBoatThread::log_headers\n%s\r\n",packet);fflush(stdout);
	// write long names
	char packetLong[6144];
	char strNormRpmLong[512];
	char strForceMomentLong[512];
	char strAhrsDataLong[512];
	char strGnssPositionDataLong[512];
	char strGnssVelocityDataLong[512];
	char strBatteryDataLong[512];
	mNormRpm.sprint_csv_long_name(strNormRpmLong);
	mForceMoment.sprint_csv_long_name(strForceMomentLong);
	mAhrsData.sprint_csv_long_name(strAhrsDataLong);
	mGnssPosData.sprint_csv_long_name(strGnssPositionDataLong);
	mGnssVelData.sprint_csv_long_name(strGnssVelocityDataLong);
	mBatteryData.sprint_csv_long_name(strBatteryDataLong);
	sprintf(packetLong,"%s%s%s%s%s%s",strNormRpmLong,strForceMomentLong,
			strAhrsDataLong,strGnssPositionDataLong,
			strGnssVelocityDataLong,strBatteryDataLong);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packetLong);
		fflush(mLogFile);
	}
	printf("LoggerBlueBoatThread::log_headers\n%s\r\n",packetLong);fflush(stdout);
}




