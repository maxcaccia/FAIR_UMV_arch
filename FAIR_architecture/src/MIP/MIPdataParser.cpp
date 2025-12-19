/*
 * MIPdataParser.cpp
 *
 *  Created on: Oct 17, 2023
 *      Author: mc
 */

#include "MIPdataParser.h"

//MIPdataParser::MIPdataParser(DataPacket *packetPtr) : DataParser(packetPtr) {
MIPdataParser::MIPdataParser(void) : DataParser() {
	g=9.80665;	// standard acceleration of gravity
				// or standard acceleration of free fall,
				// often called simply standard gravity and denoted by ɡ0 or ɡn,
				// is the nominal gravitational acceleration of an object in a vacuum near the surface of the Earth
	mDt=0.05;	// sampling time: sampling frequency is set by default to 20 Hz
	reset();
}

bool MIPdataParser::detect_header(uint8_t byte) {
	bool flag;
	mByteSync1=mByteSync2;
	mByteSync2=byte;
	flag=((mByteSync1==BYTE_SYNC1)&&(mByteSync2==BYTE_SYNC2));
	if(flag) {
//		mDataPacketPtr->append_byte(mByteSync1);
//		mDataPacketPtr->append_byte(mByteSync2);
		mDataPacket.append_byte(mByteSync1);
		mDataPacket.append_byte(mByteSync2);
		mReadBytes=2;
	}
	return flag;
}

bool MIPdataParser::build_packet(uint8_t byte) {
	mReadBytes++;
	if(mReadBytes==4)
		mPayloadLength=byte;
//	mDataPacketPtr->append_byte(byte);
	mDataPacket.append_byte(byte);
	if(mReadBytes==(mPayloadLength+6))
		return true;
	else
		return false;
}

bool MIPdataParser::decode_packet(void) {
	//
	// decode Header
	//
//	mUint8Ptr=&(mDataPacketPtr->mData[0]);mUint8Ptr++;	// SYNCH1
	mUint8Ptr=&(mDataPacket.mData[0]);mUint8Ptr++;	// SYNCH1
	mUint8Ptr++; // SYNCH2
	mDescriptorSet=*mUint8Ptr++;	// Descriptor Set
	mPayloadLength=*mUint8Ptr++;	// Payload Length
//	printf("MIPdataParser::decode_packet header %x %x %x %x\n",sync1,sync2,mDescriptorSet,mPayloadLength);
	//
	// decode packet payload
	//
	mReadBytes=0;
	while(mReadBytes<mPayloadLength) {
		//
		// decode Field
		//
		mFieldLength=*mUint8Ptr++;	// Field Length
		mReadBytes+=mFieldLength;
		mFieldDescriptor=*mUint8Ptr++;	// Field Descriptor
//		printf("MIPdataParser::decode_packet field %x %x\n",mFieldLength,mFieldDescriptor);
		if(mDescriptorSet==SENSOR_DATA) {
			switch(mFieldDescriptor) {
			case SENSOR_DATA_SCALED_ACCEL:
				mMIPdata.accel[0] = read_float(mUint8Ptr);
				mMIPdata.accel[1] = read_float(mUint8Ptr);
				mMIPdata.accel[2] = read_float(mUint8Ptr);
				break;
			case SENSOR_DATA_SCALED_GYRO:
				mMIPdata.gyro[0] = read_float(mUint8Ptr);
				mMIPdata.gyro[1] = read_float(mUint8Ptr);
				mMIPdata.gyro[2] = read_float(mUint8Ptr);
				break;
			case SENSOR_DATA_SCALED_MAG:
				mMIPdata.mag[0] = read_float(mUint8Ptr);
				mMIPdata.mag[1] = read_float(mUint8Ptr);
				mMIPdata.mag[2] = read_float(mUint8Ptr);
				break;
			case SENSOR_DATA_DELTA_THETA:
				mMIPdata.deltaTheta[0] = read_float(mUint8Ptr);
				mMIPdata.deltaTheta[1] = read_float(mUint8Ptr);
				mMIPdata.deltaTheta[2] = read_float(mUint8Ptr);
				break;
			case SENSOR_DATA_DELTA_VELOCITY:
				mMIPdata.deltaVelocity[0] = read_float(mUint8Ptr);
				mMIPdata.deltaVelocity[1] = read_float(mUint8Ptr);
				mMIPdata.deltaVelocity[2] = read_float(mUint8Ptr);
				break;
			case SENSOR_DATA_CF_EULER_ANGLES:
				mMIPdata.eulerAngles[0] = read_float(mUint8Ptr);
				mMIPdata.eulerAngles[1] = read_float(mUint8Ptr);
				mMIPdata.eulerAngles[2] = read_float(mUint8Ptr);
				break;
			case SENSOR_DATA_CF_ORIENTATION_MATRIX:
				printf("MIPdataParser::decode_packet(void) Field Descriptor %" PRIu8 " not processed!\n",mFieldDescriptor);
				for(int i=0;i<(mFieldLength-2);i++)
					mUint8Ptr++;
				break;
			case GPS_TIME_STAMP:
				mMIPdata.gpsTimeOfWeek = read_double(mUint8Ptr);
				mMIPdata.gpsWeekNumber = read_uint16(mUint8Ptr);
				mMIPdata.validFlags = read_uint16(mUint8Ptr);
//				printf("MIPdataParser::decode_packet(void) Field Descriptor %" PRIu8 " not processed!\n",mFieldDescriptor);
//				for(int i=0;i<(mFieldLength-2);i++)
//				for(int i=10;i<(mFieldLength-2);i++)
//					mUint8Ptr++;
				break;
			default:
				printf("MIPdataParser::decode_packet(void) Field Descriptor %" PRIu8 " not processed!\n",mFieldDescriptor);
				for(int i=0;i<(mFieldLength-2);i++)
					mUint8Ptr++;
				break;
			}
		}
		else {
			// mDescriptorSet != SENSOR_DATA
			printf("MIPdataParser::decode_packet(void) Descriptor Set %" PRIu8 " not processed!\n",mDescriptorSet);
			for(int i=0;i<(mFieldLength-2);i++)
				mUint8Ptr++;
		}
	} // while(mReadBytes<mPayloadLength)
	//
	// decode checksum
	//
	mChecksum = read_uint16(mUint8Ptr);
	//
	// compute Fletcher checksum
	//
//	mUint8Ptr=&(mDataPacketPtr->mData[0]);
	mUint8Ptr=&(mDataPacket.mData[0]);
	for(int i=0;i<(mPayloadLength+4);i++)
		mFletcherChecksum.add_byte(*mUint8Ptr++);

	if(mChecksum==mFletcherChecksum.fletcher_checksum(mPayloadLength+4)) {
//		printf("phi %f theta %f psi %f dpsi %f\n",
//				mMIPdata.eulerAngles[0]*180./M_PI,mMIPdata.eulerAngles[1]*180./3.14,mMIPdata.eulerAngles[2]*180./M_PI,
//				mMIPdata.deltaTheta[2]*180./M_PI);
		return true;
	}
	else {
		printf("MIPdataParser::decode_packet Checksum ERROR : ComputedChecksum %" PRIu16 " readChecksum %" PRIu16 "\n",
				mFletcherChecksum.fletcher_checksum(mPayloadLength+4),mChecksum);
		return false;
	}
}


void MIPdataParser::reset(void) {
	DataParser::reset();
	mDescriptorSet=NULL_DESCRIPTOR;
	mFieldDescriptor=NULL_DESCRIPTOR;
	mPayloadLength=0;
	mReadBytes=0;

	mUint8Ptr=NULL;
	mProcessedBytes=0;

	mFletcherChecksum.clean_checksum();
	mChecksum=0;
}


float MIPdataParser::read_float(uint8_t* &uint8ptr) {
	float dummy;
	uint8_t *ptr0;
	uint8_t *ptr;
	ptr0 = (uint8_t*) &dummy;

	ptr=ptr0+3;	*ptr=*uint8ptr++;
	ptr=ptr0+2;	*ptr=*uint8ptr++;
	ptr=ptr0+1;	*ptr=*uint8ptr++;
	ptr=ptr0+0;	*ptr=*uint8ptr++;

	return dummy;
}

uint16_t MIPdataParser::read_uint16(uint8_t* &uint8ptr) {
	uint16_t dummy;
	uint8_t *ptr0;
	uint8_t *ptr;
	ptr0 = (uint8_t*) &dummy;

	ptr=ptr0+1;	*ptr=*uint8ptr++;
	ptr=ptr0+0;	*ptr=*uint8ptr++;

	return dummy;
}

uint64_t MIPdataParser::read_uint64(uint8_t* &uint8ptr) {
	uint64_t dummy;
	uint8_t *ptr0;
	uint8_t *ptr;
	ptr0 = (uint8_t*) &dummy;

	ptr=ptr0+7;	*ptr=*uint8ptr++;
	ptr=ptr0+6;	*ptr=*uint8ptr++;
	ptr=ptr0+5;	*ptr=*uint8ptr++;
	ptr=ptr0+4;	*ptr=*uint8ptr++;
	ptr=ptr0+3;	*ptr=*uint8ptr++;
	ptr=ptr0+2;	*ptr=*uint8ptr++;
	ptr=ptr0+1;	*ptr=*uint8ptr++;
	ptr=ptr0+0;	*ptr=*uint8ptr++;

	return dummy;
}

double MIPdataParser::read_double(uint8_t* &uint8ptr) {
	double dummy;
	uint8_t *ptr0;
	uint8_t *ptr;
	ptr0 = (uint8_t*) &dummy;

	ptr=ptr0+7;	*ptr=*uint8ptr++;
	ptr=ptr0+6;	*ptr=*uint8ptr++;
	ptr=ptr0+5;	*ptr=*uint8ptr++;
	ptr=ptr0+4;	*ptr=*uint8ptr++;
	ptr=ptr0+3;	*ptr=*uint8ptr++;
	ptr=ptr0+2;	*ptr=*uint8ptr++;
	ptr=ptr0+1;	*ptr=*uint8ptr++;
	ptr=ptr0+0;	*ptr=*uint8ptr++;

	return dummy;
}


void MIPdataParser::update_data(void) {
	printf("MIPdataParser::update_data\n"); fflush(stdout);

	mAhrsData.mTimeStamp=(uint64_t)(mMIPdata.gpsTimeOfWeek*1000.);
	// angles
	mAhrsData.phi=mMIPdata.eulerAngles[0]*180/M_PI; // roll [deg]
	mAhrsData.theta=mMIPdata.eulerAngles[1]*180/M_PI; // pitch [deg]
	mAhrsData.psi=mMIPdata.eulerAngles[2]*180/M_PI; // yaw [deg]
	// linear accelerations
	mAhrsData.uDot=mMIPdata.accel[0]*g; // [m/s^2]
	mAhrsData.vDot=mMIPdata.accel[1]*g; // [m/s^2]
	mAhrsData.wDot=mMIPdata.accel[2]*g; // [m/s^2]
	// angular velocity
	mAhrsData.p=mMIPdata.gyro[0]*180/M_PI; // roll rate [deg/s]
	mAhrsData.q=mMIPdata.gyro[1]*180/M_PI; // pitch rate [deg/s]
	mAhrsData.r=mMIPdata.gyro[2]*180/M_PI; // yaw rate [deg/s]
	// magnetic components
	mAhrsData.magnX=mMIPdata.mag[0]; // [Gauss]
	mAhrsData.magnY=mMIPdata.mag[1]; // [Gauss]
	mAhrsData.magnZ=mMIPdata.mag[2]; // [Gauss]
	// linear velocity
	mAhrsData.u=mMIPdata.deltaVelocity[0]/mDt; // [m/s]
	mAhrsData.v=mMIPdata.deltaVelocity[1]/mDt; // [m/s]
	mAhrsData.w=mMIPdata.deltaVelocity[2]/mDt; // [m/s]

	mAhrsData.mValid=true;
}
