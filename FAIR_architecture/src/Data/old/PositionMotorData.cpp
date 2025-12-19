/*
 * PositionMotorData.cpp
 *
 *  Created on: 03 March 2023
 *      Author: massimo
 */
 
#include "PositionMotorData.h"
#include <stdio.h>
 
PositionMotorData::PositionMotorData(string name):TimeStampedData(name)  {
	 reset();
}

PositionMotorData::PositionMotorData(void):TimeStampedData()  {
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

	configurationStatus=0;
	operationStatus=0;

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
	// status
	configurationStatus=obj.configurationStatus;
	operationStatus=obj.operationStatus;
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
	sprintf(str,"packet_code,%s"
			"%s_position_tick,"							// No CF
			"%s_position_deg,"							// No CF
			"%s_temperature,"					// No CF
			"%s_current,"			// No CF
			"%s_fault,"					// No CF
			"%s_power,"					// No CF
			"%s_enable,"					// No CF
			"%s_configuration_status,"			// No CF
			"%s_operation_status,"			// No CF
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
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void PositionMotorData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"packet_code,%s"
			"position_ticks,"							// No CF
			"position_deg,"							// No CF
			"temperature,"							// CF
			"current,"							// ?? CF
			"fault,"							// No CF
			"power,"							// No CF
			"enable,"							// No CF
			"configuration_status,"							// No CF
			"operation_status,"							// No CF
			"power_reference,"							// No CF
			"enable_reference,"							// No CF
			"position_ticks_reference,"							// No CF
			"position_deg_reference,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int PositionMotorData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"POS_MOT %s"
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
			// status
			"%" PRIu16 " %" PRIu32 " "
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
//			// speed
//			speed,speedRpm,
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
			// status
			configurationStatus,operationStatus,
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
	 char str[1024],str1[256];

	 TimeStampedData::sprint_csv(str1);
	 sprintf(str,"POS_MOT,%s"
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
			 // status
			 "%" PRIu16 ",%" PRIu32 ","
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
			 // status
			 configurationStatus,operationStatus,
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
	char str[64];
	char name[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64 			// name timeStamp
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
			// status
			" %" SCNu16 " %" SCNu32
			// power reference
			" %" SCNu8 ""
			// enable reference
			" %" SCNu8 ""
			// position reference
			" %" SCNd32 " %lf"
			" ",
			str,
			// time stamp
			name,&mTimeStamp,
			// position
			&position,&positionDeg,
//			// speed
//			speed,speedRpm,
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
			// status
			&configurationStatus,&operationStatus,
			// power reference
			&powerCmd,
			// enable reference
			&enableCmd,
			// position
			&positionCmd,&positionDegCmd
			);
	mName=name;
}

