/*
 * SpeedMotorData.cpp
 *
 *  Created on: 02 March 2023
 *      Author: massimo
 */
 
#include "SpeedMotorData.h"
#include <stdio.h>
 
SpeedMotorData::SpeedMotorData(string name):TimeStampedData(name)  {
	 reset();
}

SpeedMotorData::SpeedMotorData(void):TimeStampedData()  {
	 reset();
}

void SpeedMotorData::reset(void) {
	speed=0;
	current=0;
	temperature=0;

	fault=0;
	power=0;
	enable=0;

	powerCmd=0;
	enableCmd=0;
	speedCmd=0;
}

SpeedMotorData & SpeedMotorData::operator = (const SpeedMotorData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	TimeStampedData::operator =(obj);
	// speed
	speed=obj.speed;
	// current
	current=obj.current;
	// temperature
	temperature=obj.temperature;
	// fault
	fault=obj.fault;
	// power
	power=obj.power;
	// enable
	enable=obj.enable;
	// power reference
	powerCmd=obj.powerCmd;
	// enable reference
	enableCmd=obj.enableCmd;
	// speed reference
	speedCmd=obj.speedCmd;
	// return a reference to myself
	return *this;
}


void SpeedMotorData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_log_name(strTS);
	sprintf(str,"packet_code,%s"
			"%s_speed,"							// No CF
			"%s_current,"					// No CF
			"%s_temperature,"					// No CF
			"%s_fault,"					// No CF
			"%s_power,"					// No CF
			"%s_enable,"					// No CF
			"%s_power_reference,"					// No CF
			"%s_enable_reference,"					// No CF
			"%s_speed_reference,"							// No CF
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
	);
	strcpy(s,str);
}

void SpeedMotorData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	TimeStampedData::sprint_csv_long_name(strTS);
	sprintf(str,"packet_code,%s"
			"speed_rpm,"							// No CF
			"current,"							// No CF
			"temperature,"							// CF
			"fault,"							// No CF
			"power,"							// No CF
			"enable,"							// No CF
			"power_reference,"							// No CF
			"enable_reference,"							// No CF
			"speed_rpm_reference,"							// No CF
			,strTS
			);
	strcpy(s,str);
}

int SpeedMotorData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	TimeStampedData::sprint(str1);
	n=sprintf(str,"SPEED_MOT %s"
			// speed
			"%.2lf "
			// currentVolt
			"%.2lf "
			// temperature
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
			// speed reference
			"%.2lf"
			" ",
			// time stamp
			str1,
			// speed
			speed,
			// current
			current,
			// temperature
			temperature,
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
			// speed reference
			speedCmd
			);
	strcpy(s,str);
	return n;
}


void SpeedMotorData::sprint_csv(char *s) {
	 char str[1024],str1[256];

	 TimeStampedData::sprint_csv(str1);
	 sprintf(str,"SPEED_MOT,%s"
			 // speed
			 "%.2lf,"
			 // currentVolt
			 "%.2lf,"
			 // temperature
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
			 // speed reference
			 "%.2lf"
			 ",",
			 // time stamp
			 str1,
			 // speed
			 speed,
			 // currentVolt
			 current,
			 // temperature
			 temperature,
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
			 // speed reference
			 speedCmd
			 );
	 strcpy(s,str);
}

void SpeedMotorData::sread(char *s) {
	char str[64];
	char name[64];
	sscanf(s,// packet_code
			"%s"
			// time stamp
			"%s %" SCNu64			// name timeStamp
			// speed
			" %lf"
			// currentVolt
			" %lf"
			// temperature
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
			// speed reference
			" %lf",
			str,
			// time stamp
			name,&mTimeStamp,
			// speed
			&speed,
			// currentVolt
			&current,
			// temperature
			&temperature,
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
			// speed reference
			&speedCmd
			);
	mName=name;
}

