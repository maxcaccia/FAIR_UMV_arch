/*
 * PlantThread.cpp
 *
 *  Created on: Feb 27, 2025
 *      Author: mc
 */

#include "PlantThread.h"

PlantThread::PlantThread(void): CntrlSimThread() {

//	printf("PlantThread::PlantThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);fflush(stdout);
	mPlant=NULL; // mPlant is instantiated in the child class <Robot>PlantThread
//	printf("PlantThread::PlantThread %s end\n",mName.c_str());fflush(stdout);
}

PlantThread::~PlantThread() {
	// TODO Auto-generated destructor stub
	if(mPlant!=NULL) {
		delete mPlant;
		mPlant=NULL;
	}
}

void PlantThread::make_thread()
{
 //   printf("PlantThread::make_thread %s start\n",mName.c_str());
    mThread = new thread(&PlantThread::execute,this);
}

bool PlantThread::init_configuration(void) {
	mPlant->set_name(mName);
//	mPlant->set_config_dir(mThreadProperties.configDir);
//
//	mPlant->reset();

	return true;
}

void PlantThread::process_command(char *cmd) {
	//	printf("******************** %s ---- PlantThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

		char chrType[64],chrCmd[64];
		string strType,strCmd;
		sscanf(cmd,"%s %s",chrType,chrCmd);
		strType=chrType;
		strCmd=chrCmd;

		if(strType=="PLANT") {
			if(mPlantCmd.decode(cmd, strCmd)) {
				if(strCmd=="ENABLE_ACTUATOR")
					process_enable_actuator(mPlantCmd.id,mPlantCmd.enable);
				else if(strCmd=="POWER_ACTUATOR")
					process_power_actuator(mPlantCmd.id,mPlantCmd.power);
				else if(strCmd=="POWER_SENSOR")
					process_power_sensor(mPlantCmd.id,mPlantCmd.power);
				else if(strCmd=="POWER_DEVICE")
					process_power_device(mPlantCmd.id,mPlantCmd.power);
				else if(strCmd=="INIT_ACTUATOR")
					process_init_actuator(mPlantCmd.id,mPlantCmd.power);
				else if(strCmd=="INIT_SENSOR")
					process_init_sensor(mPlantCmd.id,mPlantCmd.power);
				else if(strCmd=="INIT_DEVICE")
					process_init_device(mPlantCmd.id,mPlantCmd.power);

				else  {
	//				printf("TCAthread::decode ERROR read - TCA cmd is %s\n",strCmd.c_str());
	//				fflush(stdout);
					return ;
				}
			}
		} // if(strType=="PLANT")
		else  {
			printf("PlantThread::process_command ERROR read - type is %s\n",strType.c_str());
			fflush(stdout);
			return ;
		}

}


void PlantThread::process_enable_actuator(uint8_t id,uint8_t enable) {
	//	printf("******************** %s ---- PlantThread::process_enable_actuator : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	if(id==0xFFFF)
		for(int i=0;i<mPlantState.nActuators;i++)
			mPlantState.mActuatorEnableCmd[i]=enable;
	else if(id<mPlantState.nActuators)
		mPlantState.mActuatorEnableCmd[id]=enable;
	else {
		printf("PlantThread::process_enable_actuator() wrong id : %" PRIu8 "\n",id);
		fflush(stdout);
		return;
	}
	enable_actuator(id,enable);
}


void PlantThread::process_power_actuator(uint8_t id,uint8_t power) {
	//	printf("******************** %s ---- PlantThread::process_power_actuator : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	if(id==0xFFFF)
		for(int i=0;i<mPlantState.nActuators;i++)
			mPlantState.mActuatorPowerCmd[i]=power;
	else if(id<mPlantState.nActuators)
		mPlantState.mActuatorPowerCmd[id]=power;
	else {
		printf("PlantThread::process_power_actuator() wrong id : %" PRIu8 "\n",id);
		fflush(stdout);
		return;
	}
	power_actuator(id,power);
}

void PlantThread::process_power_sensor(uint8_t id,uint8_t power) {
	//	printf("******************** %s ---- PlantThread::process_power_sensor : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	if(id==0xFFFF)
		for(int i=0;i<mPlantState.nSensors;i++)
			mPlantState.mSensorPowerCmd[i]=power;
	else if(id<mPlantState.nSensors)
		mPlantState.mSensorPowerCmd[id]=power;
	else {
		printf("PlantThread::process_power_sensor() wrong id : %" PRIu8 "\n",id);
		fflush(stdout);
		return;
	}
	power_sensor(id,power);
}

void PlantThread::process_power_device(uint8_t id,uint8_t power) {
	//	printf("******************** %s ---- PlantThread::process_power_device : %s\r\n",mName.c_str(),cmd);fflush(stdout);
	if(id==0xFFFF)
		for(int i=0;i<mPlantState.nDevices;i++)
			mPlantState.mDevicePowerCmd[i]=power;
	else if(id<mPlantState.nDevices)
		mPlantState.mDevicePowerCmd[id]=power;
	else {
		printf("PlantThread::process_power_device() wrong id : %" PRIu8 "\n",id);
		fflush(stdout);
		return;
	}
	power_device(id,power);
}



void PlantThread::process_data(char *data,ssize_t nRecv) {
	mPlant->process_data(data,nRecv);
}


void PlantThread::build_data(void) {
	//
	// append output to mDataSendBuffer
	//
	Packet packet;
	if(mLogicalPositionData.mUpdated && mLogicalPositionData.mValid) {
		packet.n=mLogicalPositionData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalPositionData.mUpdated = mLogicalPositionData.mValid = false;
	//	printf("P2LSMthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalAttitudeData.mUpdated && mLogicalAttitudeData.mValid) {
		packet.n=mLogicalAttitudeData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalAttitudeData.mUpdated = mLogicalAttitudeData.mValid = false;
	//	printf("P2LSMthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalSpeedData.mUpdated && mLogicalSpeedData.mValid) {
		packet.n=mLogicalSpeedData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalSpeedData.mUpdated = mLogicalSpeedData.mValid = false;
	//	printf("P2LSMthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalLinearRateData.mUpdated && mLogicalLinearRateData.mValid) {
		packet.n=mLogicalLinearRateData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalLinearRateData.mUpdated = mLogicalLinearRateData.mValid = false;
	//	printf("P2LSMthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	if(mLogicalAngularRateData.mUpdated && mLogicalAngularRateData.mValid) {
		packet.n=mLogicalAngularRateData.sprint_cr_lf((char*)packet.data);
		mDataSendBuffer.push(packet);
		mLogicalAngularRateData.mUpdated = mLogicalAngularRateData.mValid = false;
	//	printf("P2LSMthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	}

	packet.n=mP2LSMstate.sprint_cr_lf((char*)packet.data);
	mDataSendBuffer.push(packet);
	printf("P2LSMthread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);

}


//bool P2LSMthread::read_priorities(FILE *f) {
//    //
//    // read sensor list
//    //
//    if(!read_sensor_list(f)) {
//        printf("P2LSMthread::read_priorities read_sensor_list failed!\n");fflush(stdout);
//        fclose(f);
//        return false;
//    }
//    //
//    // read Logical Position sensor priority list
//    //
//    if(!read_priority_list(f,mLogicalPosition,"LogicalPosition")) {
//        printf("P2LSMthread::read_priorities read_priority_list Logical Position failed!\n");fflush(stdout);
//        fclose(f);
//        return false;
//    }
//    //
//    // read Logical Attitude sensor priority list
//    //
//    if(!read_priority_list(f,mLogicalAttitude,"LogicalAttitude")) {
//        printf("P2LSMthread::read_priorities read_priority_list Logical Attitude failed!\n");fflush(stdout);
//        fclose(f);
//        return false;
//    }
//    //
//    // read Logical Speed sensor priority list
//    //
//    if(!read_priority_list(f,mLogicalSpeed,"LogicalSpeed")) {
//        printf("P2LSMthread::read_priorities read_priority_list Logical Speed failed!\n");fflush(stdout);
//        fclose(f);
//        return false;
//    }
//    //
//    // read Logical Linear Rate sensor priority list
//    //
//    if(!read_priority_list(f,mLogicalLinearRate,"LogicalLinearRate")) {
//        printf("P2LSMthread::read_priorities read_priority_list Logical Linear Rate failed!\n");fflush(stdout);
//        fclose(f);
//        return false;
//    }
//    //
//    // read Logical Angular Ratwe sensor priority list
//    //
//    if(!read_priority_list(f,mLogicalAngularRate,"LogicalAngularRate")) {
//        printf("P2LSMthread::read_priorities read_priority_list Logical Angular Rate failed!\n");fflush(stdout);
//        fclose(f);
//        return false;
//    }
//
////    char str[64],strDev[64],strModel[64];
////	//
////	// read logical position sensors
////	//
////	fscanf(f,"%s",str);
////    if(strcmp(str,"logicalPosition")!=0)
////    {
////        printf("P2LSMthread::read_priorities logicalPosition expected %s found!\n",str);fflush(stdout);
////        fclose(f);
////        return false;
////    }
////    bool flag=true;
////    while(flag) {
////    	//
////    	// read physical device name
////    	//
////    	fscanf(f,"%s",strDev);
////    	if(strcmp(strDev,"#end")==0)
////    		break;
////    	//
////    	// read physical device model name
////    	//
////    	fscanf(f,"%s",strModel);
////    	//
////    	// check physical device configuration file
////    	//
////    	string strDevStr=strDev;
////    	string fileName;
////        fileName=mThreadProperties.configDir+strDevStr+".cfg";
////    	FILE *fIn;
////
////    	printf("P2LSMthread::read_priorities - opening file %s\n",fileName.c_str());fflush(stdout);
////
////    	if((fIn=fopen(fileName.c_str(),"r"))==NULL) {
////    		printf("ERROR: P2LSMthread::read_priorities - opening file %s\n",fileName.c_str());fflush(stdout);
////    		fclose(f);
////    		return(false);
////    		}
////    	SensorInfo sensorInfo;
////    	if(!sensorInfo.fread(fIn)) {
////    		printf("ERROR: P2LSMthread::read_priorities - error processing %s\n",fileName.c_str());fflush(stdout);
////    		fclose(f);
////    		return(false);
////    	}
////    	fclose(fIn);
////    	//
////    	// add physical device to the list associated to logical position
////    	//
////    	//
////    	//////
////    	// add physical device sensor info
////    	bool isIn=false;
////
////    	for(long unsigned int i=0;i<mSensorInfo.size();i++) {
////    		if(mSensorInfo[i].mName==strDevStr) {
////    			isIn=true;
////    			break;
////    		}
////    	}
////    	if(!isIn) {
////    		mSensorInfo.push_back(sensorInfo);
////    	}
////
////
////    }
////
////
//
//	return true;
//}


//bool P2LSMthread::read_sensor_list(FILE *f) {
//	//
//	// read header: Sensors
//	//
//	char str[64]; //,strDev[64],strModel[64];
//	fscanf(f,"%s",str);
//	if(strcmp(str,"Sensors")!=0)
//	{
//		printf("P2LSMthread::read_sensor_list Sensors expected %s found!\n",str);fflush(stdout);
//		return false;
//	}
//	//
//	// read sensor blocks
//	//
//	char charDev[64];
//	// read device name
//	fscanf(f,"%s",charDev);
//	string strDev=charDev;
//	while(true) {
//		SensorInfo sensorInfo;
//		if(read_sensor_block(f,strDev,sensorInfo)) {
//			mSensorInfo.push_back(sensorInfo);
//			fscanf(f,"%s",charDev);
//			strDev=charDev;
//			if(strDev=="#end")
//				return true;
////			if(strDev!="#end")
////				mSensorInfo.push_back(sensorInfo);
////			else
////				return true;
//		}
//		else
//		{
//			printf("P2LSMthread::read_sensor_list read_sensor_block failed!\n");fflush(stdout);
//			return false;
//		}
//	}
//}


//bool P2LSMthread::read_sensor_block(FILE *f,string &strDev,SensorInfo &sensorInfo) {
////printf("P2LSMthread::read_sensor_block %s\n",strDev.c_str());
////	char str[64];
//
//	FILE *fIn;
//	string fileName;
//    fileName=mThreadProperties.configDir+strDev+".cfg";
//	if((fIn=fopen(fileName.c_str(),"r"))==NULL) {
//		printf("ERROR: P2LSMthread::read_sensor_block - opening file %s\n",fileName.c_str());fflush(stdout);
//		return(false);
//	}
////printf("P2LSMthread::read_sensor_block %s file open %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);
//
//	sensorInfo.mName=strDev;
//	//
//	// read device pose
//	//
//	if(!sensorInfo.fread_pose(fIn)) {
//		printf("ERROR: P2LSMthread::read_sensor_block - sensorInfo.fread_pose failed: %s\n",fileName.c_str());fflush(stdout);
//		fclose(fIn);
//		return(false);
//	}
////printf("P2LSMthread::read_sensor_block %s pose read %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);
//
//	//
//	// read list of device sensor models
//	//
//	if(!sensorInfo.fread_models(f,mThreadProperties.configDir)) {
//		printf("ERROR: P2LSMthread::read_sensor_block - sensorInfo.fread_models failed: %s\n",fileName.c_str());fflush(stdout);
//		fclose(fIn);
//		return(false);
//	}
////printf("P2LSMthread::read_sensor_block %s list of device sensor models read %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);
//
//	fclose(fIn);
//	return true;
//}


//void P2LSMthread::print_sensor_list(void) {
//	printf("Thread %s - Sensor List\n",mName.c_str());
//	for(int i=0;i<(int)mSensorInfo.size();i++)
//		mSensorInfo[i].print();
//}


//void P2LSMthread::print_configuration(void) {
//	print_sensor_list();
//	printf("Logical Position Priority List\n");
//	print_list(mLogicalPosition);
//	printf("Logical Attitude Priority List\n");
//	print_list(mLogicalAttitude);
//	printf("Logical Speed Priority List\n");
//	print_list(mLogicalSpeed);
//	printf("Logical Linear Rate Priority List\n");
//	print_list(mLogicalLinearRate);
//	printf("Logical Angular Rate Priority List\n");
//	print_list(mLogicalAngularRate);
//}
//
//
//void P2LSMthread::print_list(vector<string> list) {
//	for(int i=0;i<(int)list.size();i++)
//		printf("\t%s\n",list[i].c_str());
//}
//
//
//bool P2LSMthread::read_priority_list(FILE *f,vector<string> &list,string name) {
//	// read list name
//	char str[64];
//	fscanf(f,"%s",str);
//	if(name!=str) {
//		printf("ERROR: P2LSMthread::read_priority_list - name %s expected, but %s read\n",name.c_str(),str);fflush(stdout);
//		fclose(f);
//		return(false);
//	}
//    // read list items
//    while(true) {
//    	fscanf(f,"%s",str);
//    	if(strcmp(str,"#end")==0)
//    		return true;
//    	else {
//    		string s=str;
//    		if(is_in_sensor_list(s))
//    			list.push_back(s);
//    		else {
//    			printf("ERROR: P2LSMthread::read_priority_list - sensor name %s read, but is not in sensor list\n",s.c_str());fflush(stdout);
//    			fclose(f);
//    			return(false);
//    		}
//    	}
//    }
//
//}
//
//
//bool P2LSMthread::is_in_sensor_list(string s) {
//	for(int i=0;i<(int)mSensorInfo.size();i++)
//		if(mSensorInfo[i].mName==s)
//			return true;
//	return false;
//}



