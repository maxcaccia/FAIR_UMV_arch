/*
 * TCAstate.cpp
 *
 *  Created on:7 August 2024
 *      Author: massimo
 */
 
#include "PlantState.h"
#include <stdio.h>

PlantState::PlantState(void):TimeStampedData() {
	mPacketCode=PLANT_STATE_CODE;
	reset();
}

PlantState::PlantState(string name):TimeStampedData(name) {
	mPacketCode=PLANT_STATE_CODE;
	reset();
}

void PlantState::reset(void) {
	for(int i=0;i<N_ACTUATORS;i++)
		mActuatorEnableCmd[i]=mActuatorPowerCmd[i]=mActuatorEnableData[i]=mActuatorPowerData[i]=false;
	for(int i=0;i<N_SENSORS;i++)
		mSensorEnableCmd[i]=mSensorPowerCmd[i]=mSensorEnableData[i]=mSensorPowerData[i]=false;
	for(int i=0;i<N_DEVICES;i++)
		mDeviceEnableCmd[i]=mDevicePowerCmd[i]=mDeviceEnableData[i]=mDevicePowerData[i]=false;
}

PlantState & PlantState::operator = (const PlantState &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);

	nActuators=obj.nActuators;
	nSensors=obj.nSensors;
	nDevices=obj.nDevices;
	for(int i=0;i<N_ACTUATORS;i++) {
		mActuatorEnableCmd[i]=obj.mActuatorEnableCmd[i];
		mActuatorPowerCmd[i]=obj.mActuatorPowerCmd[i];
		mActuatorEnableData[i]=obj.mActuatorEnableData[i];
		mActuatorPowerData[i]=obj.mActuatorPowerData[i];
	}
	for(int i=0;i<N_SENSORS;i++) {
		mSensorEnableCmd[i]=obj.mSensorEnableCmd[i];
		mSensorPowerCmd[i]=obj.mSensorPowerCmd[i];
		mSensorEnableData[i]=obj.mSensorEnableData[i];
		mSensorPowerData[i]=obj.mSensorPowerData[i];
	}
	for(int i=0;i<N_DEVICES;i++) {
		mDeviceEnableCmd[i]=obj.mDeviceEnableCmd[i];
		mDevicePowerCmd[i]=obj.mDevicePowerCmd[i];
		mDeviceEnableData[i]=obj.mDeviceEnableData[i];
		mDevicePowerData[i]=false;
	}

	// return a reference to myself
	return *this;
}

void PlantState::sprint_csv_log_name(char *s) {
	printf("PlantState::sprint_csv_log_name(char *s) to be implemented!\n");fflush(stdout);
//	char strTS[256],str[2048];
//	TimeStampedData::sprint_csv_log_name(strTS);
//	sprintf(str,"%s"
//			"%s_enable,"
//			"%s_mode,"
//			"%s_function,"
//			"%s_config,"
//			,strTS
//			,mName.c_str()
//			,mName.c_str()
//			,mName.c_str()
//			,mName.c_str()
//			);
//	strcpy(s,str);
}


void PlantState::sprint_csv_long_name(char *s) {
	printf("PlantState::sprint_csv_long_name(char *s) to be implemented!\n");fflush(stdout);
//	char strTS[256],str[2048];
//	TimeStampedData::sprint_csv_long_name(strTS);
//	sprintf(str,"%s"
//			"TCA_enable,"
//			"TCA_mode,"
//			"TCA_function,"
//			"TCA_config,"
//			,strTS
//			);
//	strcpy(s,str);
}


int PlantState::sprint(char *s) {
	printf("PlantState::sprint_csv_log_name(char *s) to be implemented!\n");fflush(stdout);
	return 0;
//	char str[1024],str1[256];
//	int number;
//
//	TimeStampedData::sprint(str1);
//	number=sprintf(str,"%s"
//			// enable mode function config
//			"%d %s %s %s "
//			" ",
//			// time stamp
//			str1,
//			// enable mode function config
//			(int)mEnable,mMode.c_str(),mFunction.c_str(),mConfig.c_str()
//			);
//	strcpy(s,str);
//	return number;
}


void PlantState::sprint_csv(char *s) {
	printf("PlantState::sprint_csv(char *s) to be implemented!\n");fflush(stdout);
//	char str[1024],str1[256];
//
//	TimeStampedData::sprint_csv(str1);
//	sprintf(str,"%s"
//			// enable mode function config
//			"%d,%s,%s,%s,",
//			// time stamp
//			str1,
//			// enable mode function config
//			(int)mEnable,mMode.c_str(),mFunction.c_str(),mConfig.c_str()
//			);
//	strcpy(s,str);
}

void PlantState::sread(char *s) {
	printf("PlantState::sread(char *s) to be implemented!\n");fflush(stdout);
//	char str[64];
//	char name[64];
//	int enable;
//	uint8_t valid;
//	char mode[64],func[64],conf[64];
//	sscanf(s,// packet_code
//			"%s"
//			// time stamp
//			"%s %lf "			// name timeStamp
//			// valid flag
//			" %" SCNu8
//			// enable mode function config
//			"%d %s %s %s "
//			"",
//			str,
//			// time stamp
//			name,&mTimeStamp,&valid,
//			// enable mode function config
//			&enable,mode,func,conf
//			);
//	mName=name;
//	mValid = (bool)valid;
//	mEnable = (bool)enable;
//	mMode=mode;
//	mFunction=func;
//	mConfig=conf;
////	strcpy(s,str);
}

