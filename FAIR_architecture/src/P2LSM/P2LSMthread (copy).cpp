/*
 * P2LSMthread.cpp
 *
 *  Created on: Jan 16, 2025
 *      Author: mc
 */

#include "P2LSMthread.h"

P2LSMthread::P2LSMthread(void): CntrlSimThread() {

//	printf("P2LSMthread::P2LSMthread %s %d %d\n",mName.c_str(),mPolicy,mPriority);fflush(stdout);
	mP2LSM = new P2LSM();
//	nSensor=0;
}

P2LSMthread::~P2LSMthread() {
	// TODO Auto-generated destructor stub
}

void P2LSMthread::make_thread()
{
 //   printf("P2LSMthread::make_thread %s start\n",mName.c_str());
    mThread = new thread(&P2LSMthread::execute,this);
 //   printf("P2LSMthread::make_thread %s end\n",mName.c_str());

}

bool P2LSMthread::init_configuration(void) {
	mP2LSM->set_config_dir(mThreadProperties.configDir);

//	mP2LSM->set_sensor_info(mSensorInfo,&nSensor);
	mP2LSM->set_sensor_info(&mSensorInfo);
	mP2LSM->set_driver_interface(&mAhrsData,&mGnssPositionData,&mGnssVelocityData,&mDvlData);
	mP2LSM->set_sensor_priorities(&mLogicalPosition,&mLogicalAttitude,
			&mLogicalSpeed,&mLogicalLinearRate,&mLogicalAngularRate);
	mP2LSM->set_ngc_interface(&mLogicalPositionData,&mLogicalAttitudeData,&mLogicalSpeedData,
			&mLogicalLinearRateData,&mLogicalAngularRateData);
	mP2LSM->set_time_stamp(&mTimeStamp);

	//
	// initialise Logical Sensors: Physical Sensor priorities & sensor information
	//
printf("*************************** P2LSMthread::init_configuration \n");
	string fileName;
    fileName=mThreadProperties.configDir+mName+".cfg";
	FILE *f;
	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: P2LSMthread::init_configuration - opening file %s\n",fileName.c_str());
		return(false);
		}
    if(!read_priorities(f)) {
		printf("P2LSMthread::read_priorities failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}
    fclose(f);

    //
    // print configuration
    //
    print_configuration();

	return true;
}

void P2LSMthread::process_command(char *cmd) {
	mP2LSM->process_command(cmd);
}


void P2LSMthread::process_data(char *data,ssize_t nRecv) {
	mP2LSM->process_data(data,nRecv);
}


bool P2LSMthread::read_priorities(FILE *f) {
    //
    // read sensor list
    //
    if(!read_sensor_list(f)) {
        printf("P2LSMthread::read_priorities read_sensor_list failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Position sensor priority list
    //
    if(!read_priority_list(f,mLogicalPosition,"LogicalPosition")) {
        printf("P2LSMthread::read_priorities read_priority_list Logical Position failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Attitude sensor priority list
    //
    if(!read_priority_list(f,mLogicalAttitude,"LogicalAttitude")) {
        printf("P2LSMthread::read_priorities read_priority_list Logical Attitude failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Speed sensor priority list
    //
    if(!read_priority_list(f,mLogicalSpeed,"LogicalSpeed")) {
        printf("P2LSMthread::read_priorities read_priority_list Logical Speed failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Linear Rate sensor priority list
    //
    if(!read_priority_list(f,mLogicalLinearRate,"LogicalLinearRate")) {
        printf("P2LSMthread::read_priorities read_priority_list Logical Linear Rate failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Angular Ratwe sensor priority list
    //
    if(!read_priority_list(f,mLogicalAngularRate,"LogicalAngularRate")) {
        printf("P2LSMthread::read_priorities read_priority_list Logical Angular Rate failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }

//    char str[64],strDev[64],strModel[64];
//	//
//	// read logical position sensors
//	//
//	fscanf(f,"%s",str);
//    if(strcmp(str,"logicalPosition")!=0)
//    {
//        printf("P2LSMthread::read_priorities logicalPosition expected %s found!\n",str);fflush(stdout);
//        fclose(f);
//        return false;
//    }
//    bool flag=true;
//    while(flag) {
//    	//
//    	// read physical device name
//    	//
//    	fscanf(f,"%s",strDev);
//    	if(strcmp(strDev,"#end")==0)
//    		break;
//    	//
//    	// read physical device model name
//    	//
//    	fscanf(f,"%s",strModel);
//    	//
//    	// check physical device configuration file
//    	//
//    	string strDevStr=strDev;
//    	string fileName;
//        fileName=mThreadProperties.configDir+strDevStr+".cfg";
//    	FILE *fIn;
//
//    	printf("P2LSMthread::read_priorities - opening file %s\n",fileName.c_str());fflush(stdout);
//
//    	if((fIn=fopen(fileName.c_str(),"r"))==NULL) {
//    		printf("ERROR: P2LSMthread::read_priorities - opening file %s\n",fileName.c_str());fflush(stdout);
//    		fclose(f);
//    		return(false);
//    		}
//    	SensorInfo sensorInfo;
//    	if(!sensorInfo.fread(fIn)) {
//    		printf("ERROR: P2LSMthread::read_priorities - error processing %s\n",fileName.c_str());fflush(stdout);
//    		fclose(f);
//    		return(false);
//    	}
//    	fclose(fIn);
//    	//
//    	// add physical device to the list associated to logical position
//    	//
//    	//
//    	//////
//    	// add physical device sensor info
//    	bool isIn=false;
//
//    	for(long unsigned int i=0;i<mSensorInfo.size();i++) {
//    		if(mSensorInfo[i].mName==strDevStr) {
//    			isIn=true;
//    			break;
//    		}
//    	}
//    	if(!isIn) {
//    		mSensorInfo.push_back(sensorInfo);
//    	}
//
//
//    }
//
//

	return true;
}


bool P2LSMthread::read_sensor_list(FILE *f) {
	//
	// read header: Sensors
	//
	char str[64]; //,strDev[64],strModel[64];
	fscanf(f,"%s",str);
	if(strcmp(str,"Sensors")!=0)
	{
		printf("P2LSMthread::read_sensor_list Sensors expected %s found!\n",str);fflush(stdout);
		return false;
	}
	//
	// read sensor blocks
	//
	char charDev[64];
	// read device name
	fscanf(f,"%s",charDev);
	string strDev=charDev;
	while(true) {
		SensorInfo sensorInfo;
		if(read_sensor_block(f,strDev,sensorInfo)) {
			mSensorInfo.push_back(sensorInfo);
			fscanf(f,"%s",charDev);
			strDev=charDev;
			if(strDev=="#end")
				return true;
//			if(strDev!="#end")
//				mSensorInfo.push_back(sensorInfo);
//			else
//				return true;
		}
		else
		{
			printf("P2LSMthread::read_sensor_list read_sensor_block failed!\n");fflush(stdout);
			return false;
		}
	}
}


bool P2LSMthread::read_sensor_block(FILE *f,string &strDev,SensorInfo &sensorInfo) {
//printf("P2LSMthread::read_sensor_block %s\n",strDev.c_str());
//	char str[64];

	FILE *fIn;
	string fileName;
    fileName=mThreadProperties.configDir+strDev+".cfg";
	if((fIn=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: P2LSMthread::read_sensor_block - opening file %s\n",fileName.c_str());fflush(stdout);
		return(false);
	}
//printf("P2LSMthread::read_sensor_block %s file open %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);

	sensorInfo.mName=strDev;
	//
	// read device pose
	//
	if(!sensorInfo.fread_pose(fIn)) {
		printf("ERROR: P2LSMthread::read_sensor_block - sensorInfo.fread_pose failed: %s\n",fileName.c_str());fflush(stdout);
		fclose(fIn);
		return(false);
	}
//printf("P2LSMthread::read_sensor_block %s pose read %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);

	//
	// read list of device sensor models
	//
	if(!sensorInfo.fread_models(f,mThreadProperties.configDir)) {
		printf("ERROR: P2LSMthread::read_sensor_block - sensorInfo.fread_models failed: %s\n",fileName.c_str());fflush(stdout);
		fclose(fIn);
		return(false);
	}
//printf("P2LSMthread::read_sensor_block %s list of device sensor models read %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);

	fclose(fIn);
	return true;
}


void P2LSMthread::print_sensor_list(void) {
	printf("Thread %s - Sensor List\n",mName.c_str());
	for(int i=0;i<(int)mSensorInfo.size();i++)
		mSensorInfo[i].print();
}


void P2LSMthread::print_configuration(void) {
	print_sensor_list();
	printf("Logical Position Priority List\n");
	print_list(mLogicalPosition);
	printf("Logical Attitude Priority List\n");
	print_list(mLogicalAttitude);
	printf("Logical Speed Priority List\n");
	print_list(mLogicalSpeed);
	printf("Logical Linear Rate Priority List\n");
	print_list(mLogicalLinearRate);
	printf("Logical Angular Rate Priority List\n");
	print_list(mLogicalAngularRate);
}


void P2LSMthread::print_list(vector<string> list) {
	for(int i=0;i<(int)list.size();i++)
		printf("\t%s\n",list[i].c_str());
}


bool P2LSMthread::read_priority_list(FILE *f,vector<string> &list,string name) {
	// read list name
	char str[64];
	fscanf(f,"%s",str);
	if(name!=str) {
		printf("ERROR: P2LSMthread::read_priority_list - name %s expected, but %s read\n",name.c_str(),str);fflush(stdout);
		fclose(f);
		return(false);
	}
    // read list items
    while(true) {
    	fscanf(f,"%s",str);
    	if(strcmp(str,"#end")==0)
    		return true;
    	else {
    		string s=str;
    		if(is_in_sensor_list(s))
    			list.push_back(s);
    		else {
    			printf("ERROR: P2LSMthread::read_priority_list - sensor name %s read, but is not in sensor list\n",s.c_str());fflush(stdout);
    			fclose(f);
    			return(false);
    		}
    	}
    }

}


bool P2LSMthread::is_in_sensor_list(string s) {
	for(int i=0;i<(int)mSensorInfo.size();i++)
		if(mSensorInfo[i].mName==s)
			return true;
	return false;
}



