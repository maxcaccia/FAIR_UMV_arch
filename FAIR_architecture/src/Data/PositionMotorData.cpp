/*
 * PositionMotorData.cpp
 *
 *  Created on: 03 March 2023
 *      Author: massimo
 */
 
#include "PositionMotorData.h"
#include <stdio.h>
 
PositionMotorData::PositionMotorData(string name):TimeStampedData(name)  {
	mPacketCode=POSITION_MOTOR_CODE;
	reset();
}

PositionMotorData::PositionMotorData(void):TimeStampedData()  {
	mPacketCode=POSITION_MOTOR_CODE;
	reset();
}

void PositionMotorData::reset(void) {
	position=0;
	positionDeg=0.;
	temperature=0;
	current=0.;

	fault=0;
	power=0;
	enable=0;

	powerCmd=0;
	enableCmd=0;
	positionCmd=0;
	positionDegCmd=0;
}

PositionMotorData & PositionMotorData::operator = (const PositionMotorData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// position
	position=obj.position;
	positionDeg=obj.positionDeg;
	// temperature
	temperature=obj.temperature;
	// current
	current=obj.current;
	// fault
	fault=obj.fault;
	// power
	power=obj.power;
	// enable
	enable=obj.enable;
	//
	// references
	//
	// reference power
	powerCmd=obj.powerCmd;
	// reference enable
	enableCmd=obj.enableCmd;
	// reference position
	positionCmd=obj.positionCmd; // [ticks]: position in ticks
	positionDegCmd=obj.positionDegCmd; // [deg] : position in deg
	// return a reference to myself
	return *this;
}


void PositionMotorData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_position_tick,"							// No CF
			"%s_position_deg,"							// No CF
			"%s_temperature,"					// No CF
			"%s_current,"			// No CF
			"%s_fault,"					// No CF
			"%s_power,"					// No CF
			"%s_enable,"					// No CF
			"%s_power_reference,"			// No CF
			"%s_enable_reference,"			// No CF
			"%s_position_tick_reference,"			// No CF
			"%s_position_deg_reference,"			// No CF
			,strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void PositionMotorData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"azimuth_ticks,"							// No CF
			"azimuth_angle,"							// No CF
			"temperature,"							// CF
			"electric_current,"							// ?? CF
			"fault_signal,"							// No CF
			"relay_position,"							// No CF
			"relay_position,"							// No CF
			"relay_position,"							// No CF
			"relay_position,"							// No CF
			"azimuth_ticks,"							// No CF
			"azimuth_angle,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int PositionMotorData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"%s"
			// position
			"%" PRId32 " %.2lf "
			// temperature
			"%.2lf "
			// current
			"%.2lf "
			// fault
			"%" PRIu8 " "
			// power
			"%" PRIu8 " "
			// enable
			"%" PRIu8 " "
			// power reference
			"%" PRIu8 " "
			// enable reference
			"%" PRIu8 " "
			// position reference
			"%" PRId32 " %.2lf"
			" ",
			// time stamp
			str1,
			// position
			position,positionDeg,
			// temperature
			temperature,
			// current
			current,
			// fault
			fault,
			// power
			power,
			// enable
			enable,
			// power reference
			powerCmd,
			// enable reference
			enableCmd,
			// position
			positionCmd,positionDegCmd
			);
	strcpy(s,str);
	return n;
}


void PositionMotorData::sprint_csv(char *s) {
	char str[2048],str1[256];

	TimeStampedData::sprint_csv(str1);
	sprintf(str,"%s"
			 // position
			 "%" PRId32 ",%.2lf,"
			 // temperature
			 "%.2lf,"
			 // current
			 "%.2lf,"
			 // fault
			 "%" PRIu8 ","
			 // power
			 "%" PRIu8 ","
			 // enable
			 "%" PRIu8 ","
			 // power reference
			 "%" PRIu8 ","
			 // enable reference
			 "%" PRIu8 ","
			 // position reference
			 "%" PRId32 ",%.2lf"
			 ",",
			 // time stamp
			 str1,
			 // position
			 position,positionDeg,
			 // temperature
			 temperature,
			 // current
			 current,
			 // fault
			 fault,
			 // power
			 power,
			 // enable
			 enable,
			 // power reference
			 powerCmd,
			 // enable reference
			 enableCmd,
			 // position
			 positionCmd,positionDegCmd
			 );
	 strcpy(s,str);
}

void PositionMotorData::sread(char *s) {
	char packetCode[64];
	char name[64];
	uint8_t valid;
	sscanf(s,// packet_code
			"%s "
			// name
			"%s "
			// time stamp
			"%lf "
			// valid flag
			" %" SCNu8
			// position
			" %" SCNd32 " %lf"
			// temperature
			" %lf"
			// current
			" %lf"
			// fault
			" %" SCNu8 ""
			// power
			" %" SCNu8 ""
			// enable
			" %" SCNu8 ""
			// power reference
			" %" SCNu8 ""
			// enable reference
			" %" SCNu8 ""
			// position reference
			" %" SCNd32 " %lf"
			" ",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// position
			&position,&positionDeg,
			// temperature
			&temperature,
			// current
			&current,
			// fault
			&fault,
			// power
			&power,
			// enable
			&enable,
			// power reference
			&powerCmd,
			// enable reference
			&enableCmd,
			// position
			&positionCmd,&positionDegCmd
			);
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

