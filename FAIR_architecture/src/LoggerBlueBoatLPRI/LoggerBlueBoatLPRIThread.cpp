/*
 * LoggerBlueBoatLPRIthread.cpp
 *
 *  Created on: May 9, 2025
 *      Author: mc
 */

#include "LoggerBlueBoatLPRIThread.h"

LoggerBlueBoatLPRIthread::LoggerBlueBoatLPRIthread(void): CntrlSimThread() {
//	printf("LoggerBlueBoatThread::LoggerBlueBoatLPRIthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	mLogFile=NULL;
}

LoggerBlueBoatLPRIthread::~LoggerBlueBoatLPRIthread() {
	// TODO Auto-generated destructor stub
	if(mLogFile!=NULL) {
		fclose(mLogFile);
		mLogFile=NULL;
	}
}

void LoggerBlueBoatLPRIthread::make_thread()
{
//    printf("LoggerBlueBoatLPRIthread::make_thread %s\n",mName.c_str());
    mThread = new thread(&LoggerBlueBoatLPRIthread::execute,this);
}


bool LoggerBlueBoatLPRIthread::init_configuration(void) {
    mXYNref.set_name(mName+"Ref");
    mXYNhat.set_name(mName+"Hat");
    mXYNnormRef.set_name(mName+"Ref");
    mXYNnormHat.set_name(mName+"Hat");
    mBlueBoatRpmRef.set_name(mName+"Ref");
    mBlueBoatRpmMeas.set_name(mName+"Meas");
    mBatteryData.set_name(mName);
    mTCAstate.set_name(mName);

    mLogicalPosition.set_name(mName);
    mLogicalAttitude.set_name(mName);
    mLogicalSpeed.set_name(mName);
    mLogicalLinearRate.set_name(mName);
    mLogicalAngularRate.set_name(mName);
    mP2LSMstate.set_name(mName);

    return true;
}


void LoggerBlueBoatLPRIthread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- LoggerBlueBoatLPRIthread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);
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
//printf("start log %s\n",fileName);fflush(stdout);
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

void LoggerBlueBoatLPRIthread::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
//printf("LoggerBlueBoatLPRIthread::process_data %s\n",data);
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;

	if(dataStrType==XYN_CODE) {
		XYN xyn;
		xyn.sread(data);
		if(xyn.mName.substr(0, 3)=="ref") {
			mXYNref=xyn;
//			printf("LoggerBlueBoatLPRIthread::process_data mXYNref %s\n",mXYNref.mName.c_str());
//			fflush(stdout);
		}
		else if(xyn.mName.substr(0, 3)=="est") {
			mXYNhat=xyn;
//			printf("LoggerBlueBoatLPRIthread::process_data mXYNhat %s\n",mXYNhat.mName.c_str());
//			fflush(stdout);
		}
		else  {
			printf("LoggerBlueBoatLPRIthread::process_data WRONG xyn %s\n",xyn.mName.c_str());
			fflush(stdout);
		}
	}
	else if(dataStrType==XYN_NORM_CODE) {
		XYNnorm xyn;
		xyn.sread(data);
		if(xyn.mName.substr(0, 3)=="ref") {
			mXYNnormRef=xyn;
//			printf("LoggerBlueBoatLPRIthread::process_data mXYNnormRef %s\n",mXYNnormRef.mName.c_str());
//			fflush(stdout);
		}
		else if(xyn.mName.substr(0, 3)=="est") {
			mXYNnormHat=xyn;
//			printf("LoggerBlueBoatLPRIthread::process_data mXYNnormHat %s\n",mXYNnormHat.mName.c_str());
//			fflush(stdout);
		}
		else  {
			printf("LoggerBlueBoatLPRIthread::process_data WRONG xyn %s\n",xyn.mName.c_str());
			fflush(stdout);
		}
	}
	else if(dataStrType==NORMALISED_RPM_CODE) {
		NormalisedRpm rpm;
		rpm.sread(data);
		if(rpm.mName.substr(0, 3)=="ref") {
			mBlueBoatRpmRef=rpm;
//			printf("LoggerBlueBoatLPRIthread::process_data mBlueBoatRpmRef %s\n",mBlueBoatRpmRef.mName.c_str());
//			fflush(stdout);
		}
		else if(rpm.mName.substr(0, 3)=="msr") {
			mBlueBoatRpmMeas=rpm;
//			printf("LoggerBlueBoatLPRIthread::process_data mBlueBoatRpmMeas %s\n",mBlueBoatRpmMeas.mName.c_str());
//			fflush(stdout);
		}
		else  {
			printf("LoggerBlueBoatLPRIthread::process_data WRONG rpm %s\n",rpm.mName.c_str());
			fflush(stdout);
		}
	}
	else if(dataStrType==BATTERY_CODE)
		mBatteryData.sread(data);
	else if(dataStrType==TCA_STATE_CODE)
		mTCAstate.sread(data);
	else if(dataStrType==LOGICAL_POSITION_CODE)
		mLogicalPosition.sread(data);
	else if(dataStrType==LOGICAL_ATTITUDE_CODE)
		mLogicalAttitude.sread(data);
	else if(dataStrType==LOGICAL_SPEED_CODE)
		mLogicalSpeed.sread(data);
	else if(dataStrType==LOGICAL_LINEAR_RATE_CODE)
		mLogicalLinearRate.sread(data);
	else if(dataStrType==LOGICAL_ANGULAR_RATE_CODE)
		mLogicalAngularRate.sread(data);
	else if(dataStrType==P2LSM_STATE_CODE)
		mP2LSMstate.sread(data);

}

void LoggerBlueBoatLPRIthread::execute_thread_semantics(void) {
	//printf("LoggerBlueBoatLPRIthread::execute_thread_semantics() %s\n",mName.c_str());
	log_data();
}

void LoggerBlueBoatLPRIthread::log_data(void) {
	//printf("LoggerBlueBoatLPRIthread::log_data() %s\n",mName.c_str());
	char packet[7168];

	char strXYNref[512];
	char strXYNhat[512];
	char strXYNnormRef[512];
	char strXYNnormHat[512];
	char strRpmRef[512];
	char strRpmHat[512];
	char strBattery[512];
	char strTCAstate[512];

	char strLogicalPosition[512];
	char strLogicalAttitude[512];
	char strLogicalSpeed[512];
	char strLogicalLinearRate[512];
	char strLogicalAngularRate[512];
	char strP2LSMstate[512];

	mXYNref.sprint_csv(strXYNref);
	mXYNhat.sprint_csv(strXYNhat);
	mXYNnormRef.sprint_csv(strXYNnormRef);
	mXYNnormHat.sprint_csv(strXYNnormHat);
	mBlueBoatRpmRef.sprint_csv(strRpmRef);
	mBlueBoatRpmMeas.sprint_csv(strRpmHat);
	mBatteryData.sprint_csv(strBattery);
	mTCAstate.sprint_csv(strTCAstate);

	mLogicalPosition.sprint_csv(strLogicalPosition);
	mLogicalAttitude.sprint_csv(strLogicalAttitude);
	mLogicalSpeed.sprint_csv(strLogicalSpeed);
	mLogicalLinearRate.sprint_csv(strLogicalLinearRate);
	mLogicalAngularRate.sprint_csv(strLogicalAngularRate);
	mP2LSMstate.sprint_csv(strP2LSMstate);

	sprintf(packet,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			strXYNref,strXYNhat,strXYNnormRef,strXYNnormHat,
			strRpmRef,strRpmHat,strBattery,strTCAstate,
			strLogicalPosition,strLogicalAttitude,strLogicalSpeed,
			strLogicalLinearRate,strLogicalAngularRate,strP2LSMstate);

//	sprintf(packet,"%s%s",strNormRpm,strForceMoment);
//printf("LoggerBlueBoatLPRIthread::log_data : %s\r\n",packet);fflush(stdout);
//	printf("LoggerBlueBoatLPRIthread::log_data : %s\r\n",strForceMoment);fflush(stdout);
	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
}

void LoggerBlueBoatLPRIthread::log_headers(void) {
	//printf("LoggerBlueBoatLPRIthread::log_headers() %s\n",mName.c_str());
	char packet[7168];

	char strXYNref[512];
	char strXYNhat[512];
	char strXYNnormRef[512];
	char strXYNnormHat[512];
	char strRpmRef[512];
	char strRpmHat[512];
	char strBattery[512];
	char strTCAstate[512];
	char strLogicalPosition[512];
	char strLogicalAttitude[512];
	char strLogicalSpeed[512];
	char strLogicalLinearRate[512];
	char strLogicalAngularRate[512];
	char strP2LSMstate[512];

	//
	// write log names
	//
	mXYNref.sprint_csv_log_name(strXYNref);
	mXYNhat.sprint_csv_log_name(strXYNhat);
	mXYNnormRef.sprint_csv_log_name(strXYNnormRef);
	mXYNnormHat.sprint_csv_log_name(strXYNnormHat);
	mBlueBoatRpmRef.sprint_csv_log_name(strRpmRef);
	mBlueBoatRpmMeas.sprint_csv_log_name(strRpmHat);
	mBatteryData.sprint_csv_log_name(strBattery);
	mTCAstate.sprint_csv_log_name(strTCAstate);

	mLogicalPosition.sprint_csv_log_name(strLogicalPosition);
	mLogicalAttitude.sprint_csv_log_name(strLogicalAttitude);
	mLogicalSpeed.sprint_csv_log_name(strLogicalSpeed);
	mLogicalLinearRate.sprint_csv_log_name(strLogicalLinearRate);
	mLogicalAngularRate.sprint_csv_log_name(strLogicalAngularRate);
	mP2LSMstate.sprint_csv_log_name(strP2LSMstate);

	sprintf(packet,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			strXYNref,strXYNhat,strXYNnormRef,strXYNnormHat,
			strRpmRef,strRpmHat,strBattery,strTCAstate,
			strLogicalPosition,strLogicalAttitude,strLogicalSpeed,
			strLogicalLinearRate,strLogicalAngularRate,strP2LSMstate);

	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
//	printf("LoggerBlueBoatLPRIthread::log_headers\n%s\r\n",packet);fflush(stdout);
	//
	// write long names
	//
	mXYNref.sprint_csv_long_name(strXYNref);
	mXYNhat.sprint_csv_long_name(strXYNhat);
	mXYNnormRef.sprint_csv_long_name(strXYNnormRef);
	mXYNnormHat.sprint_csv_long_name(strXYNnormHat);
	mBlueBoatRpmRef.sprint_csv_long_name(strRpmRef);
	mBlueBoatRpmMeas.sprint_csv_long_name(strRpmHat);
	mBatteryData.sprint_csv_long_name(strBattery);
	mTCAstate.sprint_csv_long_name(strTCAstate);

	mLogicalPosition.sprint_csv_long_name(strLogicalPosition);
	mLogicalAttitude.sprint_csv_long_name(strLogicalAttitude);
	mLogicalSpeed.sprint_csv_long_name(strLogicalSpeed);
	mLogicalLinearRate.sprint_csv_long_name(strLogicalLinearRate);
	mLogicalAngularRate.sprint_csv_long_name(strLogicalAngularRate);
	mP2LSMstate.sprint_csv_long_name(strP2LSMstate);

	sprintf(packet,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			strXYNref,strXYNhat,strXYNnormRef,strXYNnormHat,
			strRpmRef,strRpmHat,strBattery,strTCAstate,
			strLogicalPosition,strLogicalAttitude,strLogicalSpeed,
			strLogicalLinearRate,strLogicalAngularRate,strP2LSMstate);

	if(mLogFile!=NULL) {
		fprintf(mLogFile,"%s\r\n",packet);
		fflush(mLogFile);
	}
//	printf("LoggerBlueBoatLPRIthread::long_headers\n%s\r\n",packet);fflush(stdout);
}




