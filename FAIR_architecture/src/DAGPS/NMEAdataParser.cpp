/*
 * NMEAdataParser.cpp
 *
 *  Created on: Oct 23, 2023
 *      Author: mc
 */

#include "NMEAdataParser.h"

//NMEAdataParser::NMEAdataParser(DataPacket *packetPtr) : DataParser(packetPtr) {
NMEAdataParser::NMEAdataParser(void) : DataParser() {
	reset();
}

bool NMEAdataParser::detect_header(uint8_t byte) {
	bool flag;
	flag=(byte==BYTE_SYNC);
	if(flag) {
//		mDataPacketPtr->append_byte(byte);
		mDataPacket.append_byte(byte);
	}
	return flag;
}

bool NMEAdataParser::build_packet(uint8_t byte) {
//	mDataPacketPtr->append_byte(byte);
	mDataPacket.append_byte(byte);
	if(byte==BYTE_END)
		return true;
	else
		return false;
}

bool NMEAdataParser::decode_packet(void) {
	bool flag;
	//
	// decode Header
	//
	mUint8Ptr=&(mDataPacket.mData[0]);
	string str;
	str=read_terminator_string(mUint8Ptr,COMMA);
	//
	// decode message
	//
	if((str=="$GPGGA")||(str=="$GNGGA")) {
		flag=decode_GGA(mUint8Ptr);
		mNMEAmessage=GGA;
	}
	else if((str=="$GPVTG")||(str=="$GNVTG")) {
		flag=decode_VTG(mUint8Ptr);
		mNMEAmessage=VTG;
	}
	else if((str=="$GPZDA")||(str=="$GNZDA")) {
		flag=decode_ZDA(mUint8Ptr);
		mNMEAmessage=ZDA;
	}
	else if((str=="$GPHDT")||(str=="$GNHDT")) {
		flag=decode_HDT(mUint8Ptr);
		mNMEAmessage=HDT;
	}
	else {
		printf("NMEAdataParser::decode_packet packet %s not processed!\n",str.c_str());
		return false;
	}

	if(flag) {
		//
		// decode checksum
		//
		int cs;
		sscanf((char*)mUint8Ptr,"%x",&cs);
		mChecksum=(uint8_t)cs;

		//
		// compute NMEA checksum
		//
		mUint8Ptr=&(mDataPacket.mData[0]);
		mUint8Ptr++;
		mNMEAchecksum=0;
		while(*mUint8Ptr!='*') {
			mNMEAchecksum ^= *mUint8Ptr;
			mUint8Ptr++;
		}
//		printf("NMEAdataParser::decode_packet checksum read %x computed %x\n",mChecksum,mNMEAchecksum);

		if(mChecksum==mNMEAchecksum)
			return true;
		else
			return false;
	}
	else
		return false;
}



void NMEAdataParser::reset(void) {
	DataParser::reset();

	mUint8Ptr=NULL;
	mNMEAmessage=NULL_MESSAGE;

	mChecksum=0;
	mNMEAchecksum=0;
}

string NMEAdataParser::read_terminator_string(uint8_t* &uint8ptr,const char cEnd) {
	string str;
	char c;
	bool tokenEnd=false;
	while(!tokenEnd) {
		c=(char)(*uint8ptr++);
		if(c==cEnd) {
			tokenEnd=true;
		}
		else {
			str.append(&c);
		}
	}
	return str;
}


bool NMEAdataParser::decode_GGA(uint8_t* &uint8ptr) {
	double valueDouble;
	string str;
	//
	// read hhmmss.ss in UTC
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mGGAdata.time=stod(str);
	else mGGAdata.time=0;
	//
	// read latitude [degmin.dddddddd]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") valueDouble=stod(str);
	else valueDouble=0.;
	// read "N"/"S"
	str=read_terminator_string(uint8ptr,COMMA);
	if((str=="N")||(str=="S")) {
		if(str=="N")
			mGGAdata.latitude=valueDouble;
		else
			mGGAdata.latitude=-valueDouble;
	}
	else
		return false;
	//
	// read longitude [degmin.dddddddd]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") valueDouble=stod(str);
	else valueDouble=0;
	// read "E"/"W"
	str=read_terminator_string(uint8ptr,COMMA);
	if((str=="E")||(str=="W")) {
		if(str=="E")
			mGGAdata.longitude=valueDouble;
		else
			mGGAdata.longitude=-valueDouble;
	}
	else
		return false;
	//
	// read position fix indicator
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mGGAdata.fixQuality=(uint8_t)stoi(str);
	else mGGAdata.fixQuality=0;
	//
	// read satellites used
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mGGAdata.nSatellites=(uint8_t)stoi(str);
	else mGGAdata.nSatellites=0;
	//
	// read HDOP
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mGGAdata.hdop=stod(str);
	else mGGAdata.hdop=0.;
	//
	// read MSL altitude [m]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mGGAdata.heightAboveMSL=stod(str);
	else mGGAdata.heightAboveMSL=0;
	// read "M"
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="M")
		return false;
	//
	// read Geoid-to-ellipsoid separation. Ellipsoid altitude = MSL Altitude + Geoid Separation
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mGGAdata.geoidSeparation=stod(str);
	else mGGAdata.geoidSeparation=0;
	// read "M"
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="M")
		return false;
	//
	// read Age of differential GPS data recor
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mGGAdata.aoDGPS=stod(str);
	else mGGAdata.aoDGPS=0;
	//
	// read Diff. Ref. Station ID
	//
	str=read_terminator_string(uint8ptr,ASTERISK);
	if(str!="") mGGAdata.dgpsStationId=(uint16_t)stoi(str);
	else mGGAdata.dgpsStationId=0;

	//printf("NMEAdataParser::decode_GGA %lf %.8lf %.8lf "
			//"%" PRIu8 " %" PRIu8 ""
			//" %lf %lf %lf %lf"
			//" %" PRIu16 "\n",
			//mGGAdata.time,mGGAdata.latitude,mGGAdata.longitude,
			//mGGAdata.fixQuality,mGGAdata.nSatellites,
			//mGGAdata.hdop,mGGAdata.heightAboveMSL,mGGAdata.geoidSeparation,mGGAdata.aoDGPS,
			//mGGAdata.dgpsStationId);

	return true;
}


bool NMEAdataParser::decode_VTG(uint8_t* &uint8ptr) {
	string str;
	//
	// read true track [deg]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mVTGdata.trackT=stod(str);
	else mVTGdata.trackT=DBL_MAX;
	// read "T"
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="T")
		return false;
	//
	// read magnetic track [deg]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mVTGdata.trackM=stod(str);
	else mVTGdata.trackM=DBL_MAX;
	// read "M"
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="M")
		return false;
	//
	// read speed [knots]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mVTGdata.speedKnots=stod(str);
	else mVTGdata.speedKnots=DBL_MAX;
	// read "N"
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="N")
		return false;
	//
	// read speed [Km/h]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mVTGdata.speedKmH=stod(str);
	else mVTGdata.speedKmH=DBL_MAX;
	// read "K"
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="K")
		return false;
	//
	// read mode
	//
	str=read_terminator_string(uint8ptr,ASTERISK);
	if(str!="")	mVTGdata.mode=str[0];
	else mVTGdata.mode='N';

//printf("NMEAdataParser::decode_VTG %lf %lf %lf %lf %c\n",
		//mVTGdata.trackT,mVTGdata.trackM,mVTGdata.speedKnots,mVTGdata.speedKmH,mVTGdata.mode);

	return true;
}


bool NMEAdataParser::decode_ZDA(uint8_t* &uint8ptr) {
	string str;
	//
	// read hhmmss.ss in UTC
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mZDAdata.time=stod(str);
	else mZDAdata.time=0;
	//
	// read day
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mZDAdata.day=(uint8_t)stoi(str);
	else mZDAdata.day=0;
	//
	// read month
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mZDAdata.month=(uint8_t)stoi(str);
	else mZDAdata.month=0;
	//
	// read year
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mZDAdata.year=(uint16_t)stoi(str);
	else mZDAdata.year=0;
	//
	// read Local time zone offset from GMT, ranging from 00 through ±13 hours
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mZDAdata.localZoneHour=(int8_t)stoi(str);
	else mZDAdata.localZoneHour=0;
	//
	// read Local time zone offset from GMT, ranging from 00 through 59 minutes
	//
	str=read_terminator_string(uint8ptr,ASTERISK);
	if(str!="") mZDAdata.localZoneMinute=(uint8_t)stoi(str);
	else mZDAdata.localZoneMinute=0;

//~ printf("NMEAdataParser::decode_ZDA %lf %" PRIu8 " %" PRIu8 " %" PRIu16 " %" PRId8 " %" PRIu8 "\n",
		//~ mZDAdata.time,mZDAdata.day,mZDAdata.month,mZDAdata.year,mZDAdata.localZoneHour,mZDAdata.localZoneMinute);

	return true;
}


bool NMEAdataParser::decode_HDT(uint8_t* &uint8ptr) {
	string str;
	//
	// read true heading [deg]
	//
	str=read_terminator_string(uint8ptr,COMMA);
	if(str!="") mHDTdata.headingT=stod(str);
	else mHDTdata.headingT=0;//DBL_MAX;
	// read "T"
	str=read_terminator_string(uint8ptr,ASTERISK);
	if(str!="T")
		return false;

	return true;
}

double NMEAdataParser::deg_min_2_deg_dec(double degMin) {
	double degMinAbs=fabs(degMin);
	double degMinSign;
	if(degMinAbs!=0)
		degMinSign=degMin/degMinAbs;
	else
		degMinSign=1.;
	double deg=floor(degMinAbs/100.);
	double min=degMinAbs-deg*100.;
	double ddd=min/60.;
	double degdddd=degMinSign*(deg+ddd);
	return degdddd;
}

void NMEAdataParser::update_data(void) {
	bool flagTrack=true;
	bool flagSpeed=true;
	switch(mNMEAmessage) {
	case GGA:
		mGnssPosData.time=mGGAdata.time;	// hhmmss.ss in UTC
		mGnssPosData.latitude=deg_min_2_deg_dec(mGGAdata.latitude); // [deg.dddddddd]
		mGnssPosData.longitude=deg_min_2_deg_dec(mGGAdata.longitude); // [deg.dddddddd]
		mGnssPosData.fixQuality=mGGAdata.fixQuality;
		mGnssPosData.nSatellites=mGGAdata.nSatellites;
		mGnssPosData.hdop=mGGAdata.hdop;
		mGnssPosData.heightAboveMSL=mGGAdata.heightAboveMSL;
		mGnssPosData.heightAboveEllipsoid=mGGAdata.heightAboveMSL+mGGAdata.geoidSeparation;
		mGnssPosData.mValid=true;
//		double aoDGPS;			// Age of differential GPS data record
//		uint16_t dgpsStationId;	// Diff. Ref. Station ID
		break;
	case VTG:
		if(mVTGdata.trackT!=DBL_MAX)
			mGnssVelData.track=mod180(mVTGdata.trackT);
		else if(mVTGdata.trackM!=DBL_MAX)
			mGnssVelData.track=mod180(mVTGdata.trackM);
		else
			flagTrack=false;
		if(mVTGdata.speedKmH!=DBL_MAX)
			mGnssVelData.speed=mVTGdata.speedKmH/3.6;
		else if(mVTGdata.speedKnots!=DBL_MAX)
			mGnssVelData.speed=mVTGdata.speedKnots*0.514444;
		else
			flagSpeed=false;
		mGnssVelData.mValid = flagTrack && flagSpeed;
		//uint8_t mode;			// mode indicator
		break;
	case ZDA:
		mGnssPosData.time=mZDAdata.time;
		mGnssPosData.date = 10000*mZDAdata.year + 100*mZDAdata.month + mZDAdata.day;
		//int8_t localZoneHour;	// Local time zone offset from GMT, ranging from 00 through ±13 hours
		//uint8_t localZoneMinute;// Local time zone offset from GMT, ranging from 00 through 59 minutes
		break;
	case HDT:
		mTrueHeadingData.trueHeading=mod180(mHDTdata.headingT);
		mTrueHeadingData.mValid=true;
		break;
	default:
		break;
	}

}

//void build_GGA_packet(uint8_t *packet,GPSdata gpsData) {
//
//}
