/*
 * MinionSimThread.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: mc
 */

#include "GnssSimThread.h"


GnssSimThread::GnssSimThread(void): CntrlSimThread() {
//	printf("GnssSimThread::GnssSimThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	dt=0.;
	mSamplingTime_ms=100;
	mPose.reset();

	mGnssSim = new GnssSim();
}

GnssSimThread::~GnssSimThread() {
	// TODO Auto-generated destructor stub
	if(mGnssSim!=NULL)
		delete mGnssSim;
}

void GnssSimThread::make_thread()
{
//    printf("GnssSimThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&GnssSimThread::execute,this);
}

bool GnssSimThread::init_configuration(void) {

	//
	// initialise dt, i.e. integration interval
	//
	dt=(double)mThreadProperties.timeValue/1000.;
	//
	// initialise GNSS device simulator
	//
	string fileName;
    fileName=mThreadProperties.configDir+mName+".cfg";
	FILE *f;
	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: GnssSimThread::init_configuration - opening file %s\n",fileName.c_str());
		return(false);
		}
    if(!read_parameters(f)) {
		printf("GnssSimThread::read_minion failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}

	//
	// set data and command name
	//
    mGnssPosData.set_name(mName);
    mGnssVelData.set_name(mName);
	return true;
}

bool GnssSimThread::read_parameters(FILE *f) {
//	//
//	// read Minion id
//	//
//	char nome[256];
//	fscanf(f,"%s %d",nome,&mId);
//	if(strcmp(nome,"id")!=0) {
//		printf("ERROR: MinionSimThread::read_parameters - read %s while expected id\n",nome);
//		return(false);
//		}
	char nome[256];
	//
	// read GNSS pose
	//
	if(!mPose.fread(f)) {
		printf("ERROR: GnssSimThread::read_parameters - read_pose(...) failed\n");
		return(false);
	}
	//
	// read GNSS sampling time
	//
	fscanf(f,"%s %" SCNu64,nome,&mSamplingTime_ms);
	if(strcmp(nome,"samplingTime_ms")!=0) {
		printf("ERROR: GnssSimThread::read_parameters - read %s while expected samplingTime_ms\n",nome);
		return(false);
		}
	//
	// read GNSS model and parameters
	//
    char str1[64],str2[64];
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"gpsModel")!=0)
    {
        printf("GnssSimThread::read_parameters gpsModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    string fileName;
    FILE *f1;
    fileName=mThreadProperties.configDir+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("GnssSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mGnssSim->read_parameters(f1)) {
    	printf("GnssSimThread::read_parameters ERROR calling mGnssSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }
    printf("GnssSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());

    fclose(f);
	return true;
}



void GnssSimThread::process_data(char *data,ssize_t nRecv) {
//printf("GnssSimThread::process_data %s\n",data);

	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;
//printf("GnssSimThread::process_data dataStrType %s\n",dataStrType.c_str());
	if(dataStrType=="LIN_POS")
		mVehPos.sread(data);
	else if(dataStrType=="ANG_POS")
		mVehAttitude.sread(data);
	else if(dataStrType=="LIN_VEL")
		mVehLinVel.sread(data);
	else if(dataStrType=="ANG_VEL")
		mVehAngVel.sread(data);
	else {
		printf("GnssSimThread::process_data unexpected data type %s\n",dataStrType.c_str());
		fflush(stdout);
	}

}

void GnssSimThread::execute_thread_semantics(void) {
//	printf("GnssSimThread::execute_thread_semantics() %s %" PRIu64 "\n",mName.c_str(),mElapsedMilliseconds);fflush(stdout);
	//
	// compute MiGNSSnion pose
	//
	compute_device_pose(mGnssPos,mGnssAttitude,mGnssLinVel,mGnssAngVel,
			mPose,
			mVehPos,mVehAttitude,mVehLinVel,mVehAngVel);

	//
	// get GPS data
	//
	mGnssSim->get_measurement(mGnssPosData,mGnssVelData,mGnssPos,mGnssLinVel,mElapsedMilliseconds);

}



void GnssSimThread::build_data(void) {
	if((mElapsedMilliseconds%mSamplingTime_ms)!=0)
		return;

	Packet packet;
	//
	// create GGA packet
	//
	packet.n=mGnssPosData.sprint_NMEA_GGA((char*)packet.data,mGnssSim->mTalkerId);
	mDataSendBuffer.push(packet);
	printf("GnssSimThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	//
	// create VTG packet
	//
	packet.n=mGnssVelData.sprint_NMEA_VTG((char*)packet.data,mGnssSim->mTalkerId);
	mDataSendBuffer.push(packet);
	printf("GnssSimThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);
	//
	// create ZDA packet
	//
	packet.n=mGnssPosData.sprint_NMEA_ZDA((char*)packet.data,mGnssSim->mTalkerId);
	mDataSendBuffer.push(packet);
	printf("GnssSimThread::build_data: %s packet %s",mName.c_str(),(char*)packet.data);


//	char strNMEA[PACKET_SIZE];
//	string str;
//	//
//	// create GGA packet
//	//
//	mGpsData.sprint_NMEA_GGA(strNMEA);
//	str=strNMEA;
//	mDataSendBuffer.push(str);
//	printf("GnssSimThread::build_data2driver: %s packet %s\n",mName.c_str(),str.c_str());
//	//
//	// create VTG packet
//	//
//	mGpsData.sprint_NMEA_VTG(strNMEA);
//	str=strNMEA;
//	mDataSendBuffer.push(str);
//	printf("GnssSimThread::build_data2driver: %s packet %s\n",mName.c_str(),str.c_str());
//	//
//	// create ZDA packet
//	//
//	mGpsData.sprint_NMEA_ZDA(strNMEA);
//	str=strNMEA;
//	mDataSendBuffer.push(str);
//	printf("GnssSimThread::build_data2driver: %s packet %s\n",mName.c_str(),str.c_str());
}




