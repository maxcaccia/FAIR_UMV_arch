/*
 * Commands.cpp
 *
 *  Created on: Feb 5, 2024
 *      Author: mc
 */

#include "Commands.h"

//
// GENERIC
//
bool GenericCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="GENERIC")) {
		printf("GenericCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="NOP") {
		command=cmd;
	}
	else {
		command="NULL";
		printf("GenericCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}
	return true;
}

int GenericCmd::code_nop(char *str) {
	int n=sprintf(str,"GENERIC NOP\r\n");
	return n;
}

//
// RIGID_BODY
//

RigidBodyCmd & RigidBodyCmd::operator = (const RigidBodyCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	actuatorId=obj.actuatorId;
	X=obj.X;
	Y=obj.Y;
	Z=obj.Z;
	K=obj.K;
	M=obj.M;
	N=obj.N;
	// return a reference to myself
	return *this;
}

bool RigidBodyCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64], strName[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="RIGID_BODY")) {
		printf("RigidBodyCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_FORCE_MOMENT") {
		sscanf(packet,"%s %s %s %lf %lf %lf %lf %lf %lf",strType,strCmd,strName,&X,&Y,&Z,&K,&M,&N);
		actuatorId=strName;
		command=cmd;
	}
	else {
		command="NULL";
		printf("RigidBodyCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}
	return true;
}

int RigidBodyCmd::code_set_force_moment(char *str) {
	int n=sprintf(str,"RIGID_BODY SET_FORCE_MOMENT %s %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\r\n",
			actuatorId.c_str(),X,Y,Z,K,M,N);
	return n;
}

//
// POS_MOT
//

PositionMotorCmd & PositionMotorCmd::operator = (const PositionMotorCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	angle=obj.angle;
	power=obj.power;
	enable=obj.enable;
	maxSpeed=obj.maxSpeed;
	// return a reference to myself
	return *this;
}

bool PositionMotorCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="POS_MOT")) {
		printf("PositionMotorCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_ANGLE") {
		sscanf(packet,"%s %s %lf",strType,strCmd,&angle);
		command=cmd;
	}
	else if(cmd=="GO_HOME") {
		command=cmd;
	}
	else if(cmd=="SET_HOME") {
		command=cmd;
	}
	else if(cmd=="POWER") {
		sscanf(packet,"%s %s %" SCNu8,strType,strCmd,&power);
		command=cmd;
	}
	else if(cmd=="ENABLE") {
		sscanf(packet,"%s %s %" SCNu8,strType,strCmd,&enable);
		command=cmd;
	}
	else if(cmd=="SET_MAX_SPEED") {
		sscanf(packet,"%s %s %lf",strType,strCmd,&maxSpeed);
		command=cmd;
	}
	else {
		command="NULL";
		printf("PositionMotorCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}


int PositionMotorCmd::code_set_angle(char *str) {
	int n=sprintf(str,"POS_MOT SET_ANGLE %.2lf\r\n",angle);
	return n;
}

int PositionMotorCmd::code_go_home(char *str) {
	int n=sprintf(str,"POS_MOT GO_HOME\r\n");
	return n;
}

int PositionMotorCmd::code_set_home(char *str) {
	int n=sprintf(str,"POS_MOT SET_HOME\r\n");
	return n;
}

int PositionMotorCmd::code_power(char *str) {
	int n=sprintf(str,"POS_MOT POWER %" PRIu8 "\r\n",power);
	return n;
}

int PositionMotorCmd::code_enable(char *str) {
	int n=sprintf(str,"POS_MOT ENABLE %" PRIu8 "\r\n",enable);
	return n;
}

int PositionMotorCmd::code_set_max_speed(char *str) {
	int n=sprintf(str,"POS_MOT SET_MAX_SPEED %.2lf\r\n",maxSpeed);
	return n;
}

//
// SPEED_MOT
//

SpeedMotorCmd & SpeedMotorCmd::operator = (const SpeedMotorCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	speed=obj.speed;
	power=obj.power;
	enable=obj.enable;
	// return a reference to myself
	return *this;
}

bool SpeedMotorCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="SPEED_MOT")) {
		printf("SpeedMotorCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_SPEED") {
		sscanf(packet,"%s %s %lf",strType,strCmd,&speed);
		command=cmd;
	}
	else if(cmd=="POWER") {
		sscanf(packet,"%s %s %" SCNu8,strType,strCmd,&power);
		command=cmd;
	}
	else if(cmd=="ENABLE") {
		sscanf(packet,"%s %s %" SCNu8,strType,strCmd,&enable);
		command=cmd;
	}
	else {
		command="NULL";
		printf("SpeedMotorCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}

int SpeedMotorCmd::code_set_speed(char *str) {
	int n=sprintf(str,"SPEED_MOT SET_SPEED %.2lf\r\n",speed);
	return n;
}

int SpeedMotorCmd::code_power(char *str) {
	int n=sprintf(str,"SPEED_MOT POWER %" PRIu8 "\r\n",power);
	return n;
}

int SpeedMotorCmd::code_enable(char *str) {
	int n=sprintf(str,"SPEED_MOT ENABLE %" PRIu8 "\r\n",enable);
	return n;
}

//
// TCA
//

TcaCmd & TcaCmd::operator = (const TcaCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	X=obj.X;
	Y=obj.Y;
	Z=obj.Z;
	K=obj.K;
	M=obj.M;
	N=obj.N;
	Xnorm=obj.Xnorm;
	Ynorm=obj.Ynorm;
	Nnorm=obj.Nnorm;
	mode=obj.mode;
	function=obj.function;
	config=obj.config;
	// return a reference to myself
	return *this;
}


bool TcaCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="TCA")) {
		printf("TcaCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="START")
		command=cmd;
	else if(cmd=="STOP")
		command=cmd;
	else if(cmd=="ENABLE")
		command=cmd;
	else if(cmd=="DISABLE")
		command=cmd;
	else if(cmd=="SET_2DOF") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&X,&N);
		Y=Z=K=M=0.;
		command=cmd;
	}
	else if(cmd=="SET_3DOF") {
		sscanf(packet,"%s %s %lf %lf %lf",strType,strCmd,&X,&Y,&N);
		Z=K=M=0.;
		command=cmd;
	}
	else if(cmd=="SET_2DOF_NORM") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&Xnorm,&Nnorm);
		Ynorm=0.;
		command=cmd;
	}
	else if(cmd=="SET_3DOF") {
		sscanf(packet,"%s %s %lf %lf %lf",strType,strCmd,&Xnorm,&Ynorm,&Nnorm);
		command=cmd;
	}
	else if(cmd=="SET_4DOF") {
		sscanf(packet,"%s %s %lf %lf %lf %lf",strType,strCmd,&X,&Y,&Z,&N);
		K=M=0.;
		command=cmd;
	}
	else if(cmd=="SET_6DOF") {
		sscanf(packet,"%s %s %lf %lf %lf %lf %lf %lf",strType,strCmd,&X,&Y,&Z,&K,&M,&N);
		command=cmd;
	}
	else if(cmd=="SET_MODE") {
		char str[64];
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		mode=str;
		command=cmd;
	}
	else if(cmd=="SET_FUNCTION") {
		char str[64];
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		function=str;
		command=cmd;
	}
	else if(cmd=="SET_CONFIG") {
		char str[64];
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		config=str;
		command=cmd;
	}
	else {
		command="NULL";
		printf("TcaCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}

int TcaCmd::code_start(char *str) {
	int n=sprintf(str,"TCA START\r\n");
	return n;
}

int TcaCmd::code_stop(char *str) {
	int n=sprintf(str,"TCA STOP\r\n");
	return n;
}

int TcaCmd::code_enable(char *str) {
	int n=sprintf(str,"TCA ENABLE\r\n");
	return n;
}

int TcaCmd::code_disable(char *str) {
	int n=sprintf(str,"TCA DISABLE\r\n");
	return n;
}

int TcaCmd::code_set_2dof(char *str) {
	int n=sprintf(str,"TCA SET_2DOF %.2lf %.2lf\r\n",X,N);
	return n;
}

int TcaCmd::code_set_3dof(char *str) {
	int n=sprintf(str,"TCA SET_3DOF %.2lf %.2lf %.2lf\r\n",X,Y,N);
	return n;
}

int TcaCmd::code_set_2dof_norm(char *str) {
	int n=sprintf(str,"TCA SET_2DOF_NORM %.2lf %.2lf\r\n",Xnorm,Nnorm);
	return n;
}

int TcaCmd::code_set_3dof_norm(char *str) {
	int n=sprintf(str,"TCA SET_3DOF_NORM %.2lf %.2lf %.2lf\r\n",Xnorm,Ynorm,Nnorm);
	return n;
}

int TcaCmd::code_set_4dof(char *str) {
	int n=sprintf(str,"TCA SET_4DOF %.2lf %.2lf %.2lf %.2lf\r\n",X,Y,Z,N);
	return n;
}

int TcaCmd::code_set_6dof(char *str) {
	int n=sprintf(str,"TCA SET_6DOF %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\r\n",X,Y,Z,K,M,N);
	return n;
}

int TcaCmd::code_set_mode(char *str) {
	int n=sprintf(str,"TCA SET_MODE %s\r\n",mode.c_str());
	return n;
}

int TcaCmd::code_set_function(char *str) {
	int n=sprintf(str,"TCA SET_FUNCTION %s\r\n",function.c_str());
	return n;
}

int TcaCmd::code_set_config(char *str) {
	int n=sprintf(str,"TCA SET_CONFIG %s\r\n",config.c_str());
	return n;
}


//
// SWAMP_TCA
//

SwampTcaCmd & SwampTcaCmd::operator = (const SwampTcaCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	n=obj.n;
	alpha=obj.alpha;
	power=obj.power;
	enable=obj.enable;
	for(int i=0;i<4;i++)
		alphaOffset[i]=obj.alphaOffset[i];
	motorType=obj.motorType;
	motorId=obj.motorId;
	// return a reference to myself
	return *this;
}

bool SwampTcaCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	char strMotorType[64],strMotorId[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="SWAMP_TCA")) {
		printf("SwampTcaCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_RPM_AZIMUTH") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&n,&alpha);
		command=cmd;
	}
	else if(cmd=="SET_POWER") {
		sscanf(packet,"%s %s %s %s %" SCNu8,strType,strCmd,strMotorType,strMotorId,&power);
		motorType=strMotorType;
		motorId=strMotorId;
		if(!((motorType=="position")||(motorType=="speed"))) {
			printf("SwampTcaCmd::decode ERROR read - motorType is %s\n",motorType.c_str());
			fflush(stdout);
			return false;
		}
		if(!((motorId=="all")||(motorId=="RL")||(motorId=="FL")||(motorId=="RR")||(motorId=="FR"))) {
			printf("SwampTcaCmd::decode ERROR read - motorId is %s\n",motorId.c_str());
			fflush(stdout);
			return false;
		}
		command=cmd;
	}
	else if(cmd=="SET_ENABLE") {
		sscanf(packet,"%s %s %s %s %" SCNu8,strType,strCmd,strMotorType,strMotorId,&enable);
		motorType=strMotorType;
		motorId=strMotorId;
		if(!((motorType=="position")||(motorType=="speed"))) {
			printf("SwampTcaCmd::decode ERROR read - motorType is %s\n",motorType.c_str());
			fflush(stdout);
			return false;
		}
		if(!((motorId=="all")||(motorId=="RL")||(motorId=="FL")||(motorId=="RR")||(motorId=="FR"))) {
			printf("SwampTcaCmd::decode ERROR read - motorId is %s\n",motorId.c_str());
			fflush(stdout);
			return false;
		}
		command=cmd;
	}
	else if(cmd=="HOMING") {
		sscanf(packet,"%s %s %s",strType,strCmd,strMotorId);
		motorId=strMotorId;
		if(!((motorId=="all")||(motorId=="RL")||(motorId=="FL")||(motorId=="RR")||(motorId=="FR"))) {
			printf("SwampTcaCmd::decode ERROR read - motorId is %s\n",motorId.c_str());
			fflush(stdout);
			return false;
		}
		command=cmd;
	}
	else if(cmd=="SET_AZIMUTH_OFFSET") {
		sscanf(packet,"%s %s %lf %lf %lf %lf",
				strType,strCmd,&alphaOffset[0],&alphaOffset[1],&alphaOffset[2],&alphaOffset[3]);
		command=cmd;
	}
	else if(cmd=="SET_AZIMUTH_HOME") {
		sscanf(packet,"%s %s %s",strType,strCmd,strMotorId);
		motorId=strMotorId;
		if(!((motorId=="all")||(motorId=="RL")||(motorId=="FL")||(motorId=="RR")||(motorId=="FR"))) {
			printf("SwampTcaCmd::decode ERROR read - motorId is %s\n",motorId.c_str());
			fflush(stdout);
			return false;
		}
		command=cmd;
	}
	else {
		command="NULL";
		printf("SwampTcaCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}

int SwampTcaCmd::code_set_rpm_azimuth(char *str) {
	int number=sprintf(str,"SWAMP_TCA SET_RPM_AZIMUTH %.2lf %.2lf\r\n",n,alpha);
	return number;
}

int SwampTcaCmd::code_set_power(char *str) {
	int number=sprintf(str,"SWAMP_TCA SET_POWER %s %s %" PRIu8 "\r\n",motorType.c_str(),motorId.c_str(),power);
	return number;
}

int SwampTcaCmd::code_set_enable(char *str) {
	int number=sprintf(str,"SWAMP_TCA SET_ENABLE %s %s %" PRIu8 "\r\n",motorType.c_str(),motorId.c_str(),enable);
	return number;
}

int SwampTcaCmd::code_homing(char *str) {
	int number=sprintf(str,"SWAMP_TCA HOMING %s\r\n",motorId.c_str());
	return number;
}

int SwampTcaCmd::code_set_azimuth_offset(char *str) {
	int number=sprintf(str,"SWAMP_TCA SET_AZIMUTH_OFFSET %.2lf %.2lf %.2lf %.2lf\r\n",
						alphaOffset[0],alphaOffset[1],alphaOffset[2],alphaOffset[3]);
	return number;
}

int SwampTcaCmd::code_set_azimuth_home(char *str) {
	int number=sprintf(str,"SWAMP_TCA SET_AZIMUTH_HOME %s\r\n",motorId.c_str());
	return number;
}



//
// BLUEBOAT_TCA
//

BlueBoatTcaCmd & BlueBoatTcaCmd::operator = (const BlueBoatTcaCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	nL=obj.nL;
	nR=obj.nR;
	n=obj.n;
	delta_n=obj.delta_n;
	// return a reference to myself
	return *this;
}

bool BlueBoatTcaCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
//	char strMotorType[64],strMotorId[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="BLUEBOAT_TCA")) {
		printf("BlueBoatTcaCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_LEFT_RIGHT_RPM") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&nL,&nR);
		command=cmd;
	}
	else if(cmd=="SET_DIFFERENTIAL_RPM") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&n,&delta_n);
		command=cmd;
	}
	else if((cmd=="SET_MANUAL_MODE")||(cmd=="SET_GUIDED_MODE")||(cmd=="ARM")||(cmd=="DISARM")||
			(cmd=="MAP_SERVO_RAW")||(cmd=="MAP_SERVO_DEFAULT")) {
		// no parameters to read
	}
	else {
		command="NULL";
		printf("BlueBoatTcaCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}

int BlueBoatTcaCmd::code_set_left_right_rpm(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA SET_LEFT_RIGHT_RPM %.2lf %.2lf\r\n",nL,nR);
	return number;
}

int BlueBoatTcaCmd::code_set_differential_rpm(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA SET_DIFFERENTIAL_RPM %.2lf %.2lf\r\n",n,delta_n);
	return number;
}

int BlueBoatTcaCmd::code_set_manual_mode(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA SET_MANUAL_MODE\r\n");
	return number;
}

int BlueBoatTcaCmd::code_set_guided_mode(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA SET_GUIDED_MODE\r\n");
	return number;
}

int BlueBoatTcaCmd::code_arm(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA ARM\r\n");
	return number;
}

int BlueBoatTcaCmd::code_disarm(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA DISARM\r\n");
	return number;
}

int BlueBoatTcaCmd::code_map_servo_raw(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA MAP_SERVO_RAW\r\n");
	return number;
}

int BlueBoatTcaCmd::code_map_servo_default(char *str) {
	int number=sprintf(str,"BLUEBOAT_TCA MAP_SERVO_DEFAULT\r\n");
	return number;
}


//
// Guidance
//

GuidanceCmd & GuidanceCmd::operator = (const GuidanceCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	latL=obj.latL;
	lonL=obj.lonL;
	gammaL=obj.gammaL;
	latWP=obj.latWP;
	lonWP=obj.lonWP;
	psiWP=obj.psiWP;
	uBar=obj.uBar;
	// return a reference to myself
	return *this;
}


bool GuidanceCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="GUIDANCE")) {
		printf("GuidanceCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_KEEP_POINT") {
		sscanf(packet,"%s %s %lf %lf %lf",strType,strCmd,&latWP,&lonWP,&psiWP);
		command=cmd;
	}
	else if(cmd=="SET_WAYPOINT") {
		sscanf(packet,"%s %s %lf %lf %lf",strType,strCmd,&latWP,&lonWP,&uBar);
		command=cmd;
	}
	else if(cmd=="SET_LINE") {
		sscanf(packet,"%s %s %lf %lf %lf %lf",strType,strCmd,&latL,&lonL,&gammaL,&uBar);
		command=cmd;
	}
	else if(cmd=="SET_PATH") {
		printf("GuidanceCmd::decode SET_PATH command format to be defined yet!\n");
		fflush(stdout);
		command=cmd;
	}
	else {
		command="NULL";
		printf("GuidanceCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}


// code commands
int GuidanceCmd::code_set_keep_point(char *str) {
	int number=sprintf(str,"GUIDANCE SET_KEEP_POINT %.8lf %.8lf %.2lf\r\n",latWP,lonWP,psiWP);
	return number;
}

int GuidanceCmd::code_set_way_point(char *str) {
	int number=sprintf(str,"GUIDANCE SET_WAY_POINT %.8lf %.8lf %.2lf\r\n",latWP,lonWP,uBar);
	return number;
}

int GuidanceCmd::code_set_line(char *str) {
	int number=sprintf(str,"GUIDANCE SET_LINE %.8lf %.8lf %.2lf %.2lf\r\n",latL,lonL,gammaL,uBar);
	return number;
}

int GuidanceCmd::code_set_path(char *str) {
	int number=sprintf(str,"GUIDANCE SET_PATH\r\n");
	return number;
}



//
// Autopilot
//

AutopilotCmd & AutopilotCmd::operator = (const AutopilotCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	psi=obj.psi;
	chi=obj.chi;
	uBar=obj.uBar;
	// return a reference to myself
	return *this;
}


bool AutopilotCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="AUTOPILOT")) {
		printf("AutopilotCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_HEADING") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&psi,&uBar);
		command=cmd;
	}
	else if(cmd=="SET_COURSE") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&chi,&uBar);
		command=cmd;
	}

	else {
		command="NULL";
		printf("AutopilotCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}


int AutopilotCmd::code_set_heading(char *str) {
	int number=sprintf(str,"AUTOPILOT SET_HEADING %.2lf %.2lf\r\n",psi,uBar);
	return number;
}


int AutopilotCmd::code_set_course(char *str) {
	int number=sprintf(str,"AUTOPILOT SET_COURSE %.2lf %.2lf\r\n",chi,uBar);
	return number;
}



//
// Control
//

ControlCmd & ControlCmd::operator = (const ControlCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	u=obj.u;
	v=obj.v;
	r=obj.r;
	// return a reference to myself
	return *this;
}


bool ControlCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="CNTRL")) {
		printf("ControlCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_SURGE") {
		sscanf(packet,"%s %s %lf",strType,strCmd,&u);
		command=cmd;
	}
	else if(cmd=="SET_SWAY") {
		sscanf(packet,"%s %s %lf",strType,strCmd,&v);
		command=cmd;
	}
	else if(cmd=="SET_YAW") {
		sscanf(packet,"%s %s %lf",strType,strCmd,&r);
		command=cmd;
	}
	else {
		command="NULL";
		printf("ControlCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}


int ControlCmd::code_set_surge(char *str) {
	int number=sprintf(str,"CONTROL SET_SURGE %.2lf\r\n",u);
	return number;
}


int ControlCmd::code_set_sway(char *str) {
	int number=sprintf(str,"CONTROL SET_SWAY %.2lf\r\n",v);
	return number;
}


int ControlCmd::code_set_yaw(char *str) {
	int number=sprintf(str,"CONTROL SET_YAW %.2lf\r\n",r);
	return number;
}


//
// Execution Control
//

ExecCntrlCmd & ExecCntrlCmd::operator = (const ExecCntrlCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	task=obj.task;
	algo=obj.algo;
	// return a reference to myself
	return *this;
}


bool ExecCntrlCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="EXEC_CNTRL")) {
		printf("ExecCntrlCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	char str[256],str2[256];
	if(cmd=="ENABLE_GUIDANCE") {
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		task=str;
		command=cmd;
	}
	else if(cmd=="DISABLE_GUIDANCE") {
		sscanf(packet,"%s %s",strType,strCmd);
		command=cmd;
	}
	else if(cmd=="ENABLE_AUTOPILOT") {
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		task=str;
		command=cmd;
	}
	else if(cmd=="DISABLE_AUTOPILOT") {
		sscanf(packet,"%s %s",strType,strCmd);
		command=cmd;
	}
	else if(cmd=="ENABLE_CONTROL") {
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		task=str;
		command=cmd;
	}
	else if(cmd=="DISABLE_CONTROL") {
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		task=str;
		command=cmd;
	}
	else if(cmd=="SET_GUIDANCE_ALGO") {
		sscanf(packet,"%s %s %s %s",strType,strCmd,str,str2);
		task=str;
		algo=str2;
		command=cmd;
	}
	else if(cmd=="SET_AUTOPILOT_ALGO") {
		sscanf(packet,"%s %s %s %s",strType,strCmd,str,str2);
		task=str;
		algo=str2;
		command=cmd;
	}
	else if(cmd=="SET_CONTROL_ALGO") {
		sscanf(packet,"%s %s %s %s",strType,strCmd,str,str2);
		task=str;
		algo=str2;
		command=cmd;
	}
	else {
		command="NULL";
		printf("ExecCntrlCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}


int ExecCntrlCmd::code_enable_guidance(char *str) {
	int number=sprintf(str,"EXEC_CNTRL ENABLE_GUIDANCE %s\r\n",task.c_str());
	return number;
}


int ExecCntrlCmd::code_disable_guidance(char *str) {
	int number=sprintf(str,"EXEC_CNTRL DISABLE_GUIDANCE\r\n");
	return number;
}


int ExecCntrlCmd::code_enable_autopilot(char *str) {
	int number=sprintf(str,"EXEC_CNTRL ENABLE_AUTOPILOT %s\r\n",task.c_str());
	return number;
}


int ExecCntrlCmd::code_disable_autopilot(char *str) {
	int number=sprintf(str,"EXEC_CNTRL DISABLE_AUTOPILOT\r\n");
	return number;
}


int ExecCntrlCmd::code_enable_control(char *str) {
	int number=sprintf(str,"EXEC_CNTRL ENABLE_CONTROL %s\r\n",task.c_str());
	return number;
}


int ExecCntrlCmd::code_disable_control(char *str) {
	int number=sprintf(str,"EXEC_CNTRL DISABLE_CONTROL %s\r\n",task.c_str());
	return number;
}


int ExecCntrlCmd::code_set_guidance_algo(char *str) {
	int number=sprintf(str,"EXEC_CNTRL SET_GUIDANCE_ALGO %s %s\r\n",task.c_str(),algo.c_str());
	return number;
}


int ExecCntrlCmd::code_set_autopilot_algo(char *str) {
	int number=sprintf(str,"EXEC_CNTRL SET_AUTOPILOT_ALGO %s %s\r\n",task.c_str(),algo.c_str());
	return number;
}


int ExecCntrlCmd::code_set_control_algo(char *str) {
	int number=sprintf(str,"EXEC_CNTRL SET_CONTROL_ALGO %s %s\r\n",task.c_str(),algo.c_str());
	return number;
}

//
// PLANT
//

PlantCmd & PlantCmd::operator = (const PlantCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	power=obj.power;
	enable=obj.enable;
	id=obj.id;
	timeout=obj.timeout;
	// return a reference to myself
	return *this;
}

bool PlantCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="PLANT")) {
		printf("PlantCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="ENABLE_ACTUATOR") {
		sscanf(packet,"%s %s %" SCNu8 " %" SCNu8,strType,strCmd,&id,&enable);
		command=cmd;
	}
	else if(cmd=="POWER_ACTUATOR") {
		sscanf(packet,"%s %s %" SCNu8 " %" SCNu8,strType,strCmd,&id,&power);
		command=cmd;
	}
	else if(cmd=="POWER_SENSOR") {
		sscanf(packet,"%s %s %" SCNu8 " %" SCNu8,strType,strCmd,&id,&power);
		command=cmd;
	}
	else if(cmd=="POWER_DEVICE") {
		sscanf(packet,"%s %s %" SCNu8 " %" SCNu8,strType,strCmd,&id,&power);
		command=cmd;
	}
	else if(cmd=="INIT_ACTUATOR") {
		sscanf(packet,"%s %s %" SCNu8 " %lf",strType,strCmd,&id,&timeout);
		command=cmd;
	}
	else if(cmd=="INIT_SENSOR") {
		sscanf(packet,"%s %s %" SCNu8 " %lf",strType,strCmd,&id,&timeout);
		command=cmd;
	}
	else if(cmd=="INIT_DEVICE") {
		sscanf(packet,"%s %s %" SCNu8 " %lf",strType,strCmd,&id,&timeout);
		command=cmd;
	}
	else {
		command="NULL";
		printf("PlantCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}


int PlantCmd::code_enable_actuator(char *str) {
	int n=sprintf(str,"PLANT ENABLE_ACTUATOR %" PRIu8 " %" PRIu8 "\r\n",id,enable);
	return n;
}

int PlantCmd::code_power_actuator(char *str) {
	int n=sprintf(str,"PLANT POWER_ACTUATOR %" PRIu8 " %" PRIu8 "\r\n",id,enable);
	return n;
}

int PlantCmd::code_power_sensor(char *str) {
	int n=sprintf(str,"PLANT POWER_SENSOR %" PRIu8 " %" PRIu8 "\r\n",id,enable);
	return n;
}

int PlantCmd::code_power_device(char *str) {
	int n=sprintf(str,"PLANT POWER_DEVICE %" PRIu8 " %" PRIu8 "\r\n",id,enable);
	return n;
}

int PlantCmd::code_init_actuator(char *str) {
	int n=sprintf(str,"PLANT INIT_ACTUATOR %" PRIu8 " %lf\r\n",id,timeout);
	return n;
}

int PlantCmd::code_init_sensor(char *str) {
	int n=sprintf(str,"PLANT INIT_SENSOR %" PRIu8 " %lf\r\n",id,timeout);
	return n;
}

int PlantCmd::code_init_device(char *str) {
	int n=sprintf(str,"PLANT INIT_DEVICE %" PRIu8 " %lf\r\n",id,timeout);
	return n;
}


BlueBoatCmd & BlueBoatCmd::operator = (const BlueBoatCmd &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	mode=obj.mode;
	arm=obj.arm;
//	thrust=obj.thrust;
//	torque=obj.torque;
	leftRpm=obj.leftRpm;
	rightRpm=obj.rightRpm;
	lat=obj.lat;
	lon=obj.lon;
	// return a reference to myself
	return *this;
}

bool BlueBoatCmd::decode(char *packet,string &command) {
	char strType[64], strCmd[64];
	string tipo,cmd;
	sscanf(packet,"%s %s",strType,strCmd);
	tipo=strType;
	if(!(tipo=="BLUE_BOAT")) {
		printf("BlueBoatCmd::decode ERROR read - type is %s\n",tipo.c_str());
		fflush(stdout);
		return false;
	}
	cmd=strCmd;
	if(cmd=="SET_MODE") {
		char str[64];
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		mode=str;
		command=cmd;
	}
	else if(cmd=="SET_ARM") {
		char str[64];
		sscanf(packet,"%s %s %s",strType,strCmd,str);
		arm=str;
		command=cmd;
	}
//	else if(cmd=="SET_THRUST_TORQUE") {
//		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&thrust,&torque);
//		command=cmd;
//	}
	else if(cmd=="SET_MANUAL_REF") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&leftRpm,&rightRpm);
		command=cmd;
	}
	else if(cmd=="MAP_SERVO_RAW") {
		command=cmd;
	}
	else if(cmd=="MAP_SERVO_DEFAULT") {
		command=cmd;
	}
	else if(cmd=="SET_WAYPOINT") {
		sscanf(packet,"%s %s %lf %lf",strType,strCmd,&lat,&lon);
		command=cmd;
	}
	else {
		command="NULL";
		printf("BlueBoatCmd::decode ERROR read - command is %s\n",cmd.c_str());
		fflush(stdout);
		return false;
	}

	return true;
}

int BlueBoatCmd::code_set_mode(char *str) {
	int number=sprintf(str,"BLUE_BOAT SET_MODE %s\r\n",mode.c_str());
	return number;
}

int BlueBoatCmd::code_set_arm(char *str) {
	int number=sprintf(str,"BLUE_BOAT SET_ARM %s\r\n",arm.c_str());
	return number;
}

//int BlueBoatCmd::code_set_thrust_torque(char *str) {
//	int number=sprintf(str,"BLUE_BOAT SET_THRUST_TORQUE %.2lf %.2lf\r\n",thrust,torque);
//	return number;
//}

int BlueBoatCmd::code_set_manual_ref(char *str) {
	int number=sprintf(str,"BLUE_BOAT SET_MANUAL_REF %.2lf %.2lf\r\n",leftRpm,rightRpm);
	return number;
}

int BlueBoatCmd::code_map_servo_raw(char *str) {
	int number=sprintf(str,"BLUE_BOAT MAP_SERVO_RAW\r\n");
	return number;
}

int BlueBoatCmd::code_map_servo_default(char *str) {
	int number=sprintf(str,"BLUE_BOAT MAP_SERVO_DEFAULT\r\n");
	return number;
}

int BlueBoatCmd::code_set_waypoint(char *str) {
	int number=sprintf(str,"BLUE_BOAT SET_WAYPOINT %.8lf %.8lf\r\n",lat,lon);
	return number;
}
