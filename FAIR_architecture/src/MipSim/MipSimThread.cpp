/*
 * MipSimThread.cpp
 *
 *  Created on: Feb 13, 2024
 *      Author: mc
 */

#include "MipSimThread.h"


MipSimThread::MipSimThread(void): CntrlSimThread() {
//	printf("MipSimThread::GnssSimThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	dt=0.;
	mSamplingTime_ms=100;
	mPose.reset();

	mAhrsSim = new AhrsSim();

	g=9.80665;	// standard acceleration of gravity
}

MipSimThread::~MipSimThread() {
	// TODO Auto-generated destructor stub
	if(mAhrsSim!=NULL)
		delete mAhrsSim;
}

void MipSimThread::make_thread()
{
//    printf("MipSimThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&MipSimThread::execute,this);
}

bool MipSimThread::init_configuration(void) {

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
		printf("ERROR: MipSimThread::init_configuration - opening file %s\n",fileName.c_str());
		return(false);
		}
    if(!read_parameters(f)) {
		printf("MipSimThread::read_minion failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}

	//
	// set data and command name
	//
    mAhrsData.set_name(mName);
	return true;
}

bool MipSimThread::read_parameters(FILE *f) {
	char nome[256];
	//
	// read AHRS pose
	//
	if(!mPose.fread(f)) {
		printf("ERROR: MipSimThread::read_parameters - read_pose(...) failed\n");
		return(false);
	}
	//
	// read AHRS sampling time
	//
	fscanf(f,"%s %" SCNu64,nome,&mSamplingTime_ms);
	if(strcmp(nome,"samplingTime_ms")!=0) {
		printf("ERROR: MipSimThread::read_parameters - read %s while expected samplingTime_ms\n",nome);
		return(false);
		}
	//
	// read AHRS model and parameters
	//
    char str1[64],str2[64];
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"ahrsModel")!=0)
    {
        printf("MipSimThread::read_parameters ahrsModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    string fileName;
    FILE *f1;
    fileName=mThreadProperties.configDir+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("MipSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mAhrsSim->read_parameters(f1)) {
    	printf("MipSimThread::read_parameters ERROR calling mGpsSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }
    printf("MipSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());

    fclose(f);
	return true;
}



void MipSimThread::process_data(char *data,ssize_t nRecv) {
//printf("MipSimThread::process_data nRecv %d data %s\n",(int)nRecv,data);
	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;
//printf("MipSimThread::process_data dataStrType %s\n",dataStrType.c_str());
	if(dataStrType=="LIN_POS")
		mVehPos.sread(data);
	else if(dataStrType=="ANG_POS")
		mVehAttitude.sread(data);
	else if(dataStrType=="LIN_VEL")
		mVehLinVel.sread(data);
	else if(dataStrType=="ANG_VEL")
		mVehAngVel.sread(data);
	else {
		printf("MipSimThread::process_data unexpected data type %s\n",dataStrType.c_str());
		fflush(stdout);
	}
}

void MipSimThread::execute_thread_semantics(void) {
//	printf("MipSimThread::execute_thread_semantics() %s %" PRIu64 "\n",mName.c_str(),mElapsedMilliseconds);fflush(stdout);
	//
	// compute MIP pose
	//
//	compute_device_pose(mMipState,mPose,mVehicleState);
	compute_device_pose(mMipPos,mMipAttitude,mMipLinVel,mMipAngVel,
			mPose,
			mVehPos,mVehAttitude,mVehLinVel,mVehAngVel);
	//
	// get AHRS data
	//
	if(mAhrsSim->get_measurement(mAhrsData,mMipAttitude,mMipLinVel,mMipAngVel,mElapsedMilliseconds)) {
		mAhrsData.mTimeStamp=mTimeStamp;
		mAhrsData.mValid=true;
	}
	else
		mAhrsData.mValid=false;
}



void MipSimThread::build_data(void) {
	if((mElapsedMilliseconds%mSamplingTime_ms)!=0)
		return;

char str[512];
mAhrsData.sprint(str);
printf("MipSimThread::build_data %s\n",str);

	Packet packet;
	packet.n=build_packet(packet.data);
	mDataSendBuffer.push(packet);
}

int MipSimThread::build_packet(uint8_t* uint8ptr) {
	float value;
	uint8_t* uint8ptr0=uint8ptr;
	FletcherChecksum fletcherChecksum;

	uint8_t payloadLength=(uint8_t)0x38;
	//
	// build Header
	//
	*uint8ptr=0x75;uint8ptr++;	// SYNCH1
	*uint8ptr=0x65;uint8ptr++; // SYNCH2
	*uint8ptr=0x80;uint8ptr++;	// Descriptor Set
	*uint8ptr=payloadLength; uint8ptr++;	// Payload Length
	//
	// build SCALED_ACC
	//
	*uint8ptr=0x0E; uint8ptr++;	// Field Length
	*uint8ptr=0x04; uint8ptr++;	// Field Descriptor
	// uDot
	value=(float)mAhrsData.uDot/g;
	build_float(uint8ptr,value);
	// vDot
	value=(float)mAhrsData.vDot/g;
	build_float(uint8ptr,value);
	// wDot
	value=(float)mAhrsData.wDot/g;
	build_float(uint8ptr,value);
	//
	// build SCALED_GYRO
	//
	*uint8ptr=0x0E; uint8ptr++;	// Field Length
	*uint8ptr=0x05; uint8ptr++;	// Field Descriptor
	// p
	value=(float)mAhrsData.p*M_PI/180.;
	build_float(uint8ptr,value);
	// q
	value=(float)mAhrsData.q*M_PI/180.;
	build_float(uint8ptr,value);
	// r
	value=(float)mAhrsData.r*M_PI/180.;
	build_float(uint8ptr,value);
	//
	// build SCALED_MAG
	//
	*uint8ptr=0x0E; uint8ptr++;	// Field Length
	*uint8ptr=0x06; uint8ptr++;	// Field Descriptor
	// magnX
	value=(float)mAhrsData.magnX;
	build_float(uint8ptr,value);
	// magnY
	value=(float)mAhrsData.magnY;
	build_float(uint8ptr,value);
	// magnZ
	value=(float)mAhrsData.magnZ;
	build_float(uint8ptr,value);
	//
	// build CF_EULER_ANGLES
	//
	*uint8ptr=0x0E; uint8ptr++;	// Field Length
	*uint8ptr=0x0C; uint8ptr++;	// Field Descriptor
	// phi
	value=(float)mAhrsData.phi*M_PI/180.;
	build_float(uint8ptr,value);
	// theta
	value=(float)mAhrsData.theta*M_PI/180.;
	build_float(uint8ptr,value);
	// psi
	value=(float)mAhrsData.psi*M_PI/180.;
	build_float(uint8ptr,value);
	//
	// compute Fletcher checksum
	//
	uint8_t* ptr=uint8ptr0;
	for(int i=0;i<(payloadLength+4);i++)
		fletcherChecksum.add_byte(*ptr++);

	uint16_t checksum;

	checksum=fletcherChecksum.fletcher_checksum(payloadLength+4);

	build_uint16(uint8ptr,checksum);

	// additional \r\n for string delimiter in UDP packet: they do not exist in actual data stream
	*uint8ptr=(uint8_t)'\r'; uint8ptr++;	// \r
	*uint8ptr=(uint8_t)'\n'; uint8ptr++;	// \n

//printf("==================================================================================\n");
//ptr=uint8ptr0;
//for(int i=0;i<(4+payloadLength+2);i++) printf("0x%x\n",*ptr++);
	int length=4+payloadLength+2;
	return length;
}

void MipSimThread::build_float(uint8_t* &uint8ptr,float value) {
	float dummy=value;
	uint8_t *ptr0;
	uint8_t *ptr;
	ptr0 = (uint8_t*) &dummy;
	ptr=ptr0+3;	*uint8ptr=*ptr; uint8ptr++;
	ptr=ptr0+2;	*uint8ptr=*ptr; uint8ptr++;
	ptr=ptr0+1;	*uint8ptr=*ptr; uint8ptr++;
	ptr=ptr0+0;	*uint8ptr=*ptr; uint8ptr++;
}

void MipSimThread::build_uint16(uint8_t* &uint8ptr,uint16_t value) {
	uint16_t dummy=value;
	uint8_t *ptr0;
	uint8_t *ptr;
	ptr0 = (uint8_t*) &dummy;
	ptr=ptr0+1;	*uint8ptr=*ptr; uint8ptr++;
	ptr=ptr0+0;	*uint8ptr=*ptr; uint8ptr++;
}

