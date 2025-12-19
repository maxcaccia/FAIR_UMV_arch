/*
 * P2LSM.cpp
 *
 *  Created on: 16 Jan 2025
 *      Author: massimo
 */

#include "P2LSM.h"

P2LSM::P2LSM(void) {
//	printf("P2LSM::P2LSM %s begin\n",mName.c_str());fflush(stdout);
	mName="";
	mConfigDir="";
    //
    // sensor information (name, pose, measurement uncertainty)
    //
    mSensorInfo = NULL;
//    nSensor = NULL;
    //
    // Driver Interface - sensor data: AHRS, GNSS-GGA, GNSS-VTG, DVL
    //
    mAhrsData = NULL;
    mGnssPositionData = NULL;
    mGnssVelocityData = NULL;
    mDvlData = NULL;
    //
    // Logical Sensors: Physical Sensor priorities
    //
    mLogicalPosition = NULL;
    mLogicalAttitude = NULL;
    mLogicalSpeed = NULL;
    mLogicalLinearRate = NULL;
    mLogicalAngularRate = NULL;
    //
    // NGC interface - Logical data
    //
    mLogicalPositionData = NULL;
    mLogicalAttitudeData = NULL;
    mLogicalSpeedData = NULL;
    mLogicalLinearRateData = NULL;
    mLogicalAngularRateData = NULL;

//    //
//    // active Logical Sensors
//    //
//	mP2LSMstate->mActiveLogicalPosition="null";
//    mP2LSMstate->mActiveLogicalAttitude="null";
//    mP2LSMstate->mActiveLogicalSpeed="null";
//    mP2LSMstate->mActiveLogicalLinearRate="null";
//    mP2LSMstate->mActiveLogicalAngularRate="null";
    //
    // elapsed time for each Logical Sensor
    //
    mElapsedTimeLogicalPosition=0;
    mElapsedTimeLogicalAttitude=0;
    mElapsedTimeLogicalSpeed=0;
    mElapsedTimeLogicalLinearRate=0;
    mElapsedTimeLogicalAngularRate=0;
    //
    // priority for each active Logical Sensor
    //
    mPriorityLogicalPosition=LOWEST_PRIORITY;
    mPriorityLogicalAttitude=LOWEST_PRIORITY;
    mPriorityLogicalSpeed=LOWEST_PRIORITY;
    mPriorityLogicalLinearRate=LOWEST_PRIORITY;
    mPriorityLogicalAngularRate=LOWEST_PRIORITY;

    mTimeStamp=NULL;

//    printf("P2LSM::P2LSM %s end\n",mName.c_str());fflush(stdout);
}

P2LSM::~P2LSM(void) {

}


void P2LSM::execute(void) {
////	printf("P2LSM::execute(): %s\n",(*mLogicalPositionStr)[0].c_str()); fflush(stdout);
//	printf("P2LSM::execute():\n"); fflush(stdout);

}


void P2LSM::process_command(char *cmd,ssize_t n) {
printf("P2LSM::process_command()\n"); fflush(stdout);

}


bool P2LSM::process_data(char *data,ssize_t nRecv) {
//	printf("P2LSM::process_data() %s\n",data); fflush(stdout);

	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;
	bool flag=false;
	if(dataStrType==AHRS_CODE) {
		mAhrsData->sread(data);
		flag=process_ahrs_data();
	}
	else if(dataStrType==GNSS_POSITION_CODE) {
		mGnssPositionData->sread(data);
		flag=process_gnss_gga_data();
	}
	else if(dataStrType==GNSS_VELOCITY_CODE) {
		mGnssVelocityData->sread(data);
		flag=process_gnss_vtg_data();
	}
	else if(dataStrType==DVL_CODE) {
		mDvlData->sread(data);
		flag=process_dvl_data();
	}
	else {
//		printf("P2LSM::process_data unexpected data type %s\n",dataStrType.c_str());
////		printf("P2LSM::process_data unexpected data type %s\n%s\n",dataStrType.c_str(),data);
		fflush(stdout);
	}

	return flag;
}


bool P2LSM::process_ahrs_data(void) {
//	printf("P2LSM::process_ahrs_data\n");
	//
	// check if device is in the sensor list
	//
	if(!is_in_sensor_list(mAhrsData->mName)) {
//		printf("P2LSM::process_ahrs_data %s is not in sensor list\n",mAhrsData->mName.c_str());
//		fflush(stdout);
		return false;
	}

	//
	// process device as Logical Attitude
	//
	// get priority
	uint8_t priority=get_priority(mAhrsData->mName,*mLogicalAttitude);
	// check active priority
	if(priority<LOWEST_PRIORITY) {
		if (priority <= mPriorityLogicalAttitude) {
			//
			// set active Logical Attitude
			//
			mP2LSMstate->mActiveLogicalAttitude=mAhrsData->mName;
			mPriorityLogicalAttitude=priority;
			mElapsedTimeLogicalAttitude=0;
			//
			// assign data to active Logical Attitude
			//
			// assign BodyAttitudeData
			BodyAttitudeData bodyAttitudeData;
			// name
			bodyAttitudeData.mName=mP2LSMstate->mActiveLogicalAttitude;
			// attitude values
			bodyAttitudeData.assign(*mAhrsData);
			SensorInfo sensorInfo;
			if(!get_sensor_info(sensorInfo,mAhrsData->mName)) {
				printf("P2LSM::process_ahrs_data %s is not in sensor list\n",mAhrsData->mName.c_str());
				fflush(stdout);
				return false;
			}
			// attitude measurement uncertainty
			double phiStd,thetaStd,yawStd;
			bool flag=true;
			flag = flag && sensorInfo.get_value(phiStd,"roll_stdev");
			flag = flag && sensorInfo.get_value(thetaStd,"pitch_stdev");
			flag = flag && sensorInfo.get_value(yawStd,"yaw_stdev");
			if(flag) bodyAttitudeData.assign(phiStd, thetaStd, yawStd);
			//
			// assign Logical Data
			//
			// body attitude data
			mLogicalAttitudeData->assign(bodyAttitudeData);
			// pose
			mLogicalAttitudeData->assign(sensorInfo.mPose);
			// set Attitude Logical Data as updated and valid
			mLogicalAttitudeData->mUpdated=true;
			mLogicalAttitudeData->mValid=true;
		}
	}
	//
	// update elapsed time and check timeout
	//
	mElapsedTimeLogicalAttitude+=dt;
	if (mElapsedTimeLogicalAttitude >= SENSOR_TIMEOUT) {
	// activeSensor is cleaned
	mP2LSMstate->mActiveLogicalAttitude = "null";
	mPriorityLogicalAttitude = LOWEST_PRIORITY;
	}

	//char s[512];
	//mLogicalAttitudeData->sprint(s);
	//printf("P2LSM::process_ahrs_data %s\n",s);
	//fflush(stdout);

	//
	// process device as Logical Angular Rate
	//
	// get priority
	priority=get_priority(mAhrsData->mName,*mLogicalAngularRate);
	// check active priority
	if(priority<LOWEST_PRIORITY) {
		if (priority <= mPriorityLogicalAngularRate) {
			//
			// set active Logical AngularRate
			//
			mP2LSMstate->mActiveLogicalAngularRate=mAhrsData->mName;
			mPriorityLogicalAngularRate=priority;
			mElapsedTimeLogicalAngularRate=0;
			//
			// assign data to active Logical AngularRate
			//
			// assign BodyAngularRateData
			BodyAngularRateData bodyAngularRateData;
			// name
			bodyAngularRateData.mName=mP2LSMstate->mActiveLogicalAngularRate;
			// angular rate values
			bodyAngularRateData.assign(*mAhrsData);
			SensorInfo sensorInfo;
			if(!get_sensor_info(sensorInfo,mAhrsData->mName)) {
				printf("P2LSM::process_ahrs_data %s is not in sensor list\n",mAhrsData->mName.c_str());
				fflush(stdout);
				return false;
			}
			// angular rate measurement uncertainty
			double phiRateStd,thetaRateStd,yawRateStd;
			bool flag=true;
			flag = flag && sensorInfo.get_value(phiRateStd,"roll_rate_stdev");
			flag = flag && sensorInfo.get_value(thetaRateStd,"pitch_rate_stdev");
			flag = flag && sensorInfo.get_value(yawRateStd,"yaw_rate_stdev");
			if(flag) bodyAngularRateData.assign(phiRateStd, thetaRateStd, yawRateStd);
			//
			// assign Logical Data
			//
			// body AngularRate data
			mLogicalAngularRateData->assign(bodyAngularRateData);
			// pose
			mLogicalAngularRateData->assign(sensorInfo.mPose);
			// set AngularRate Logical Data as updated and valid
			mLogicalAngularRateData->mUpdated=true;
			mLogicalAngularRateData->mValid=true;
		}
	}
	//
	// update elapsed time and check timeout
	//
	mElapsedTimeLogicalAngularRate+=dt;
	if (mElapsedTimeLogicalAngularRate >= SENSOR_TIMEOUT) {
	// activeSensor is cleaned
	mP2LSMstate->mActiveLogicalAngularRate = "null";
	mPriorityLogicalAngularRate = LOWEST_PRIORITY;
	}

	//mLogicalAngularRateData->sprint(s);
	//printf("P2LSM::process_ahrs_data %s\n",s);
	//fflush(stdout);

	//
	// process device as Logical Linear Rate
	//
	// get priority
	priority=get_priority(mAhrsData->mName,*mLogicalLinearRate);
	// check active priority
	if(priority<LOWEST_PRIORITY) {
		if (priority <= mPriorityLogicalLinearRate) {
			//
			// set active Logical LinearRate
			//
			mP2LSMstate->mActiveLogicalLinearRate=mAhrsData->mName;
			mPriorityLogicalLinearRate=priority;
			mElapsedTimeLogicalLinearRate=0;
			//
			// assign data to active Logical LinearRate
			//
			// assign BodyLinearRateData
			BodyLinearRateData bodyLinearRateData;
			// name
			bodyLinearRateData.mName=mP2LSMstate->mActiveLogicalLinearRate;
			// linear rate values
			bodyLinearRateData.assign(*mAhrsData);
			SensorInfo sensorInfo;
			if(!get_sensor_info(sensorInfo,mAhrsData->mName)) {
				printf("P2LSM::process_ahrs_data %s is not in sensor list\n",mAhrsData->mName.c_str());
				fflush(stdout);
				return false;
			}
			// linear rate measurement uncertainty
			double uStd,vStd,wStd;
			bool flag=true;
			flag = flag && sensorInfo.get_value(uStd,"surge_rate_stdev");
			flag = flag && sensorInfo.get_value(vStd,"sway_rate_stdev");
			flag = flag && sensorInfo.get_value(wStd,"heave_rate_stdev");
			if(flag) bodyLinearRateData.assign(uStd, vStd, wStd);
			//
			// assign Logical Data
			//
			// body AngularRate data
			mLogicalLinearRateData->assign(bodyLinearRateData);
			// pose
			mLogicalLinearRateData->assign(sensorInfo.mPose);
			// set LinearRate Logical Data as updated and valid
			mLogicalLinearRateData->mUpdated=true;
			mLogicalLinearRateData->mValid=true;
		}
	}
	//
	// update elapsed time and check timeout
	//
	mElapsedTimeLogicalLinearRate+=dt;
	if (mElapsedTimeLogicalLinearRate >= SENSOR_TIMEOUT) {
	// activeSensor is cleaned
	mP2LSMstate->mActiveLogicalLinearRate = "null";
	mPriorityLogicalLinearRate = LOWEST_PRIORITY;
	}

	return true;
}


bool P2LSM::process_gnss_gga_data(void) {
	//
	// check if device is in the sensor list
	//
	if(!is_in_sensor_list(mGnssPositionData->mName)) {
//		printf("P2LSM::process_gnss_gga_data %s is not in sensor list\n",mGnssPositionData->mName.c_str());
		fflush(stdout);
		return false;
	}

	//
	// process device as Logical Position
	//
	// get priority
	uint8_t priority=get_priority(mGnssPositionData->mName,*mLogicalPosition);
	// check active priority
	if(priority<LOWEST_PRIORITY) {
		if (priority <= mPriorityLogicalPosition) {
			//
			// set active Logical Position
			//
			mP2LSMstate->mActiveLogicalPosition=mGnssPositionData->mName;
			mPriorityLogicalPosition=priority;
			mElapsedTimeLogicalPosition=0;
			//
			// assign data to active Logical Position
			//
			// assign BodyPositionData
			BodyPositionData bodyPositionData;
			// name
			bodyPositionData.mName=mP2LSMstate->mActiveLogicalPosition;
			// position and default uncertainty values
			bodyPositionData.assign(*mGnssPositionData);
			SensorInfo sensorInfo;
			if(!get_sensor_info(sensorInfo,mGnssPositionData->mName)) {
				printf("P2LSM::process_gnss_gga_data %s is not in sensor list\n",mGnssPositionData->mName.c_str());
				fflush(stdout);
				return false;
			}
			// update position measurement uncertainty (based on the sensor model)
			double latStdFactor,lonStdFactor;
			bool flag=true;
			flag = flag && sensorInfo.get_value(latStdFactor,"latitude_stdev");
			flag = flag && sensorInfo.get_value(lonStdFactor,"longitude_stdev");
			if(flag) {
				bodyPositionData.latStd*=latStdFactor;
				bodyPositionData.lonStd*=lonStdFactor;
			}
			//
			// assign Logical Data
			//
			// body position data
			mLogicalPositionData->assign(bodyPositionData);
			// pose
			mLogicalPositionData->assign(sensorInfo.mPose);
			// set Position Logical Data as updated and valid
			mLogicalPositionData->mUpdated=true;
			mLogicalPositionData->mValid=true;
		}
	}
	//
	// update elapsed time and check timeout
	//
	mElapsedTimeLogicalPosition+=dt;
	if (mElapsedTimeLogicalPosition >= SENSOR_TIMEOUT) {
	// activeSensor is cleaned
	mP2LSMstate->mActiveLogicalPosition = "null";
	mPriorityLogicalPosition = LOWEST_PRIORITY;
	}

	//char s[512];
	//mLogicalPositionData->sprint(s);
	//printf("P2LSM::process_gnss_gga_data %s\n",s);
	//fflush(stdout);

	return true;
}


bool P2LSM::process_gnss_vtg_data(void) {
	//
	// check if device is in the sensor list
	//
	if(!is_in_sensor_list(mGnssVelocityData->mName)) {
//		printf("P2LSM::process_gnss_vtg_data %s is not in sensor list\n",mGnssVelocityData->mName.c_str());
		fflush(stdout);
		return false;
	}

	//
	// process device as Logical Speed
	//
	// get priority
	uint8_t priority=get_priority(mGnssVelocityData->mName,*mLogicalSpeed);
	// check active priority
	if(priority<LOWEST_PRIORITY) {
		if (priority <= mPriorityLogicalSpeed) {
			//
			// set active Logical Speed
			//
			mP2LSMstate->mActiveLogicalSpeed=mGnssVelocityData->mName;
			mPriorityLogicalSpeed=priority;
			mElapsedTimeLogicalSpeed=0;
			//
			// assign data to active Logical Speed
			//
			// assign BodyPositionData
			BodySpeedData bodySpeedData;
			// name
			bodySpeedData.mName=mP2LSMstate->mActiveLogicalSpeed;
			// speed values
			bodySpeedData.assign(*mGnssVelocityData);
			SensorInfo sensorInfo;
			if(!get_sensor_info(sensorInfo,mGnssVelocityData->mName)) {
				printf("P2LSM::process_gnss_vtg_data %s is not in sensor list\n",mGnssVelocityData->mName.c_str());
				fflush(stdout);
				return false;
			}
			// speed and course measurement uncertainty
			double speedStd,courseStd;
			bool flag=true;
			flag = flag && sensorInfo.get_value(speedStd,"speed_stdev");
			flag = flag && sensorInfo.get_value(courseStd,"course_stdev");
			if(flag) bodySpeedData.assign(speedStd, courseStd);
			//
			// assign Logical Data
			//
			// body speed data
			mLogicalSpeedData->assign(bodySpeedData);
			// pose
			mLogicalSpeedData->assign(sensorInfo.mPose);
			// set Speed Logical Data as updated and valid
			mLogicalSpeedData->mUpdated=true;
			mLogicalSpeedData->mValid=true;
		}
	}
	//
	// update elapsed time and check timeout
	//
	mElapsedTimeLogicalSpeed+=dt;
	if (mElapsedTimeLogicalSpeed >= SENSOR_TIMEOUT) {
	// activeSensor is cleaned
	mP2LSMstate->mActiveLogicalSpeed = "null";
	mPriorityLogicalSpeed = LOWEST_PRIORITY;
	}

	//char s[512];
	//mLogicalSpeedData->sprint(s);
	//printf("P2LSM::process_gnss_vtg_data %s\n",s);
	//fflush(stdout);

	return true;
}


bool P2LSM::process_dvl_data(void) {
	//
	// check if device is in the sensor list
	//
	if(!is_in_sensor_list(mDvlData->mName)) {
//		printf("P2LSM::process_dvl_data %s is not in sensor list\n",mDvlData->mName.c_str());
		fflush(stdout);
		return false;
	}

	//
	// process device as Logical Linear Rate
	//
	// get priority
	uint8_t priority=get_priority(mDvlData->mName,*mLogicalLinearRate);
	// check active priority
	if(priority<LOWEST_PRIORITY) {
		if (priority <= mPriorityLogicalLinearRate) {
			//
			// set active Logical LinearRate
			//
			mP2LSMstate->mActiveLogicalLinearRate=mDvlData->mName;
			mPriorityLogicalLinearRate=priority;
			mElapsedTimeLogicalLinearRate=0;
			//
			// assign data to active Logical LinearRate
			//
			// assign BodyLinearRateData
			BodyLinearRateData bodyLinearRateData;
			// name
			bodyLinearRateData.mName=mP2LSMstate->mActiveLogicalLinearRate;
			// linear rate values
			bodyLinearRateData.assign(*mDvlData);
			SensorInfo sensorInfo;
			if(!get_sensor_info(sensorInfo,mDvlData->mName)) {
				printf("P2LSM::process_dvl_data %s is not in sensor list\n",mDvlData->mName.c_str());
				fflush(stdout);
				return false;
			}
			// linear rate measurement uncertainty
			double uStd,vStd,wStd;
			bool flag=true;
			flag = flag && sensorInfo.get_value(uStd,"surge_rate_stdev");
			flag = flag && sensorInfo.get_value(vStd,"sway_rate_stdev");
			flag = flag && sensorInfo.get_value(wStd,"heave_rate_stdev");
			if(flag) bodyLinearRateData.assign(uStd, vStd, wStd);
			//
			// assign Logical Data
			//
			// body AngularRate data
			mLogicalLinearRateData->assign(bodyLinearRateData);
			// pose
			mLogicalLinearRateData->assign(sensorInfo.mPose);
			// set LinearRate Logical Data as updated and valid
			mLogicalLinearRateData->mUpdated=true;
			mLogicalLinearRateData->mValid=true;
		}
	}
	//
	// update elapsed time and check timeout
	//
	mElapsedTimeLogicalLinearRate+=dt;
	if (mElapsedTimeLogicalLinearRate >= SENSOR_TIMEOUT) {
	// activeSensor is cleaned
	mP2LSMstate->mActiveLogicalLinearRate = "null";
	mPriorityLogicalLinearRate = LOWEST_PRIORITY;
	}

	//char s[512];
	//mLogicalLinearRateData->sprint(s);
	//printf("P2LSM::process_dvl_data %s\n",s);
	//fflush(stdout);

	return true;
}



bool P2LSM::read_configuration(void) {
	//
	// initialise Logical Sensors: Physical Sensor priorities & sensor information
	//
//	printf("*************************** P2LSM::read_configuration \n");
	string fileName;
	fileName=mConfigDir+mName+"-P2LSMparam.cfg";
	FILE *f;
	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: P2LSM::read_configuration - opening file %s\n",fileName.c_str());
		return(false);
	}
	if(!read_priorities(f)) {
		printf("P2LSM::read_priorities failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}
	fclose(f);

	//
	// print configuration
	//
	print_configuration();

	return true;
}


bool P2LSM::read_priorities(FILE *f) {
    //
    // read sensor list
    //
    if(!read_sensor_list(f)) {
        printf("P2LSM::read_priorities read_sensor_list failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Position sensor priority list
    //
    if(!read_priority_list(f,*mLogicalPosition,"LogicalPosition")) {
        printf("P2LSM::read_priorities read_priority_list Logical Position failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Attitude sensor priority list
    //
    if(!read_priority_list(f,*mLogicalAttitude,"LogicalAttitude")) {
        printf("P2LSM::read_priorities read_priority_list Logical Attitude failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Speed sensor priority list
    //
    if(!read_priority_list(f,*mLogicalSpeed,"LogicalSpeed")) {
        printf("P2LSM::read_priorities read_priority_list Logical Speed failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Linear Rate sensor priority list
    //
    if(!read_priority_list(f,*mLogicalLinearRate,"LogicalLinearRate")) {
        printf("P2LSM::read_priorities read_priority_list Logical Linear Rate failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // read Logical Angular Rate sensor priority list
    //
    if(!read_priority_list(f,*mLogicalAngularRate,"LogicalAngularRate")) {
        printf("P2LSM::read_priorities read_priority_list Logical Angular Rate failed!\n");fflush(stdout);
        fclose(f);
        return false;
    }

	return true;
}


bool P2LSM::read_sensor_list(FILE *f) {
	//
	// read header: Sensors
	//
	char str[64]; //,strDev[64],strModel[64];
	fscanf(f,"%s",str);
	if(strcmp(str,"Sensors")!=0)
	{
		printf("P2LSM::read_sensor_list Sensors expected %s found!\n",str);fflush(stdout);
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
			mSensorInfo->push_back(sensorInfo);
			fscanf(f,"%s",charDev);
			strDev=charDev;
			if(strDev=="#end")
				return true;
		}
		else
		{
			printf("P2LSM::read_sensor_list read_sensor_block failed!\n");fflush(stdout);
			return false;
		}
	}
}


bool P2LSM::read_sensor_block(FILE *f,string &strDev,SensorInfo &sensorInfo) {
//printf("P2LSM::read_sensor_block %s\n",strDev.c_str());
//	char str[64];

	FILE *fIn;
	string fileName;
    fileName=mConfigDir+strDev+".cfg";
	if((fIn=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: P2LSM::read_sensor_block - opening file %s\n",fileName.c_str());fflush(stdout);
		return(false);
	}
//printf("P2LSM::read_sensor_block %s file open %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);

	sensorInfo.mName=strDev;
	//
	// read device pose
	//
	if(!sensorInfo.fread_pose(fIn)) {
		printf("ERROR: P2LSM::read_sensor_block - sensorInfo.fread_pose failed: %s\n",fileName.c_str());fflush(stdout);
		fclose(fIn);
		return(false);
	}
//printf("P2LSM::read_sensor_block %s pose read %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);

	//
	// read list of device sensor models
	//
	if(!sensorInfo.fread_models(f,mConfigDir)) {
		printf("ERROR: P2LSM::read_sensor_block - sensorInfo.fread_models failed: %s\n",fileName.c_str());fflush(stdout);
		fclose(fIn);
		return(false);
	}
//printf("P2LSM::read_sensor_block %s list of device sensor models read %s\n",strDev.c_str(),fileName.c_str());fflush(stdout);

	fclose(fIn);
	return true;
}


void P2LSM::print_sensor_list(void) {
	printf("Thread %s - Sensor List\n",mName.c_str());
	for(int i=0;i<(int)mSensorInfo->size();i++)
		(*mSensorInfo)[i].print();
}


void P2LSM::print_configuration(void) {
	print_sensor_list();
	printf("Logical Position Priority List\n");
	print_list(*mLogicalPosition);
	printf("Logical Attitude Priority List\n");
	print_list(*mLogicalAttitude);
	printf("Logical Speed Priority List\n");
	print_list(*mLogicalSpeed);
	printf("Logical Linear Rate Priority List\n");
	print_list(*mLogicalLinearRate);
	printf("Logical Angular Rate Priority List\n");
	print_list(*mLogicalAngularRate);
}


void P2LSM::print_list(vector<string> list) {
	for(int i=0;i<(int)list.size();i++)
		printf("\t%s\n",list[i].c_str());
}


bool P2LSM::read_priority_list(FILE *f,vector<string> &list,string name) {
	// read list name
	char str[64];
	fscanf(f,"%s",str);
	if(name!=str) {
		printf("ERROR: P2LSM::read_priority_list - name %s expected, but %s read\n",name.c_str(),str);fflush(stdout);
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
    			if(s!="none") {
    				printf("ERROR: P2LSM::read_priority_list - sensor name %s read, but is not in sensor list\n",s.c_str());fflush(stdout);
    				fclose(f);
    				return(false);
    			}
    		}
    	}
    }

}


bool P2LSM::is_in_sensor_list(string s) {
	for(int i=0;i<(int)mSensorInfo->size();i++)
		if((*mSensorInfo)[i].mName==s)
			return true;
	return false;
}


bool P2LSM::get_sensor_info(SensorInfo &sensorInfo,string name) {
	for(int i=0;i<(int)mSensorInfo->size();i++)
		if((*mSensorInfo)[i].mName==name) {
			sensorInfo=(*mSensorInfo)[i];
			return true;
		}
	return false;
}


uint8_t P2LSM::get_priority(string name,vector<string> list) {
	for(uint8_t i=0;i<(uint8_t)list.size();i++)
		if(list[i]==name)
			return i;
	return LOWEST_PRIORITY;
}

