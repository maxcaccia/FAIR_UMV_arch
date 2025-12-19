/*
 * MinionSimThread.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: mc
 */

#include "MinionSimThread.h"


MinionSimThread::MinionSimThread(void): CntrlSimThread() {
//	printf("MinionSimThread::MinionSimThread %s %d %d\n",mName.c_str(),mPolicy,mPriority);
	dt=0.;
	mId=0;
	mSamplingTime_ms=100;
	mDigitalInput=mDigitalOutput=0;
	mBatteryVoltage=BATTERY_VOLTAGE_FULL;
	mPose.reset();

	mGnssSim = new GnssSim();
	mAhrsSim = new AhrsSim();
	mSpeedMotorSim = new SpeedMotorSim();
	mPositionMotorSim = new PositionMotorSim();

}

MinionSimThread::~MinionSimThread() {
	// TODO Auto-generated destructor stub
	if(mGnssSim!=NULL)
		delete mGnssSim;
	if(mAhrsSim!=NULL)
		delete mAhrsSim;
	if(mSpeedMotorSim!=NULL)
		delete mSpeedMotorSim;
	if(mPositionMotorSim!=NULL)
		delete mPositionMotorSim;
}

void MinionSimThread::make_thread()
{
//    printf("MinionSimThread::make_thread %s\n",mName.c_str());
    mThread = new thread(&MinionSimThread::execute,this);
}

bool MinionSimThread::init_configuration(void) {

	//
	// initialise dt, i.e. integration interval
	//
	dt=(double)mThreadProperties.timeValue/1000.;
	//
	// initialise Minion simulator
	//
	string fileName;
    fileName=mThreadProperties.configDir+mName+".cfg";
	FILE *f;
	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: MinionSimThread::init_configuration - opening file %s\n",fileName.c_str());
		return(false);
		}
    if(!read_parameters(f)) {
		printf("MinionSimThread::read_minion failed! %s %s\n",mName.c_str(),fileName.c_str());fflush(stdout);
		return false;
	}

	//
	// set data and command name
	//
    mGnssPosData.set_name(mName);
    mGnssVelData.set_name(mName);
    mAhrsData.set_name(mName);

	mSpeedMotorData.set_name(mName); // pump-jet motor
	mPositionMotorData.set_name(mName); // azimuth motor

//	mForceTorque.set_name(mName);
	mRbCmd.set_name(mName);
	return true;
}

bool MinionSimThread::read_parameters(FILE *f) {
	//
	// read Minion id
	//
	char nome[256];
	fscanf(f,"%s %d",nome,&mId);
	if(strcmp(nome,"id")!=0) {
		printf("ERROR: MinionSimThread::read_parameters - read %s while expected id\n",nome);
		return(false);
		}
	//
	// read Minion pose
	//
	if(!mPose.fread(f)) {
		printf("ERROR: MinionSimThread::read_parameters - read_pose(...) failed\n");
		return(false);
	}
	//
	// read Minion sampling time
	//
	fscanf(f,"%s %" SCNu64,nome,&mSamplingTime_ms);
	if(strcmp(nome,"samplingTime_ms")!=0) {
		printf("ERROR: MinionSimThread::read_parameters - read %s while expected samplingTime_ms\n",nome);
		return(false);
		}
	//
	// read GPS model and parameters
	//
    char str1[64],str2[64];
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"gpsModel")!=0)
    {
        printf("MinionSimThread::read_parameters gpsModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    string fileName;
    FILE *f1;
    fileName=mThreadProperties.configDir+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("MinionSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mGnssSim->read_parameters(f1)) {
    	printf("MinionSimThread::read_parameters ERROR calling mGnssSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }
    printf("MinionSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());
	//
	// read AHRS model and parameters
	//
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"ahrsModel")!=0)
    {
        printf("MinionSimThread::read_parameters ahrsModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    fileName=mThreadProperties.configDir+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("MinionSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mAhrsSim->read_parameters(f1)) {
    	printf("MinionSimThread::read_parameters ERROR calling mAhrsSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }
    printf("MinionSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());
	//
	// read Pump Motor model and parameters
	//
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"pumpMotorModel")!=0)
    {
        printf("MinionSimThread::read_parameters pumpMotorModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    fileName=mThreadProperties.configDir+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("MinionSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mSpeedMotorSim->read_parameters(f1)) {
    	printf("MinionSimThread::read_parameters ERROR calling speedMotorSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }

    printf("MinionSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());
	//
	// read Azimuth Motor model and parameters
	//
	fscanf(f,"%s %s",str1,str2);
    if(strcmp(str1,"azimuthMotorModel")!=0)
    {
        printf("MinionSimThread::read_parameters azimuthMotorModel expected %s found!\n",str1);fflush(stdout);
        fclose(f);
        return false;
    }
    fileName=mThreadProperties.configDir+str2+".cfg";
    if((f1=fopen(fileName.c_str(),"r"))==NULL) {
    	printf("MinionSimThread::read_parameters ERROR opening file %s\n",fileName.c_str());fflush(stdout);
    	fclose(f);
    	return false;
    }
    if(!mPositionMotorSim->read_parameters(f1)) {
    	printf("MinionSimThread::read_parameters ERROR calling positiondMotorSim->read_parameters(%s)\n",fileName.c_str());fflush(stdout);
    	return false;
    }
    else {
    	fclose(f1);
    }

    printf("MinionSimThread::read_parameters(.) %s %s\n",mName.c_str(),fileName.c_str());

    fclose(f);
	return true;
}


void MinionSimThread::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- MinionSimThread::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	int32_t cmd0,cmd1,cmd2;
	double speedRefPercent;
    sscanf(cmd,"%" SCNd32 " %" SCNd32 " %" SCNd32 , &cmd0,&cmd1,&cmd2);
//        printf("MinionSimThread::process_command() : %d %d %d\n",cmd0,cmd1,cmd2);fflush(stdout);
    switch(cmd0){
        case COMMAND_NOP_OPCODE:
//            	printf("MinionSimThread::process_command() : COMMAND_NOP_OPCODE\n");fflush(stdout);
            break;
        case COMMAND_SET_DIGITAL_CHANNEL_OPCODE:
//            	printf("MinionSimThread::process_command() : COMMAND_SET_DIGITAL_CHANNEL_OPCODE\n");fflush(stdout);
            mDigitalOutput ^= (- cmd2 ^ mDigitalOutput) & (1UL << cmd1);
            break;
        case COMMAND_SET_ANALOG_CHANNEL_OPCODE:
//            	printf("MinionSimThread::process_command() : COMMAND_SET_ANALOG_CHANNEL_OPCODE\n");fflush(stdout);
            break;
        case COMMAND_MOTOR_POWER_OPCODE:
            switch(cmd1) {
                case THRUST_MOTOR:
//                    	printf("MinionSimThread::process_command() : COMMAND_MOTOR_POWER_OPCODE THRUST_MOTOR\n");fflush(stdout);
                	mSpeedMotorSim->set_power(cmd2);
                	if(cmd2==1)
                		mDigitalOutput = mDigitalOutput | (0x0001 << 4);
                	else
                		mDigitalOutput = mDigitalOutput & ~(0x0001 << 4);
                    break;
                case AZIMUTHAL_MOTOR:
//                    	printf("MinionSimThread::process_command() : COMMAND_MOTOR_POWER_OPCODE AZIMUTHAL_MOTOR\n");fflush(stdout);
                	mPositionMotorSim->set_power(cmd2);
                	if(cmd2==1)
                		mDigitalOutput = mDigitalOutput | (0x0001 << 6);
                	else
                		mDigitalOutput = mDigitalOutput & ~(0x0001 << 6);
                    break;
                default:
                    printf("MinionSimThread::process_command() error %d %d %d\n",cmd0,cmd1,cmd2);
                    fflush(stdout);
                    break;
            }
            break;
        case COMMAND_MOTOR_ENABLE_OPCODE:
            switch(cmd1) {
                case THRUST_MOTOR:
//                    	printf("MinionSimThread::process_command() : COMMAND_MOTOR_ENABLE_OPCODE THRUST_MOTOR\n");fflush(stdout);
                	mSpeedMotorSim->set_enable(cmd2);
                	if(cmd2==1)
                		mDigitalOutput = mDigitalOutput | (0x0001 << 5);
                	else
                		mDigitalOutput = mDigitalOutput & (~(0x0001 << 5));
                    break;
                case AZIMUTHAL_MOTOR:
//                    	printf("MinionSimThread::process_command() : COMMAND_MOTOR_ENABLE_OPCODE AZIMUTHAL_MOTOR\n");fflush(stdout);
                	mPositionMotorSim->set_enable(cmd2);
                    break;
                default:
                    printf("MinionSimThread::process_command() error %d %d %d\n",cmd0,cmd1,cmd2);
                    fflush(stdout);
                    break;
            }
            break;
        case COMMAND_MOTOR_SET_REFERENCE_OPCODE:
            switch(cmd1) {
                case THRUST_MOTOR:
//                    	printf("MinionSimThread::process_command() : COMMAND_MOTOR_SET_REFERENCE_OPCODE THRUST_MOTOR\n");fflush(stdout);
                	speedRefPercent=double(cmd2)/1000.;
                	mSpeedMotorSim->set_speed(THRUST_RPM_MAX*speedRefPercent/100.);
                    break;
                case AZIMUTHAL_MOTOR:
//                    	printf("MinionSimThread::process_command() : COMMAND_MOTOR_SET_REFERENCE_OPCODE AZIMUTHAL_MOTOR\n");fflush(stdout);
                	mPositionMotorSim->set_position(cmd2);
                    break;
                default:
                    printf("MinionSimThread::process_command() COMMAND_MOTOR_SET_REFERENCE_OPCODE error %s %d %d %d\n",mName.c_str(),cmd0,cmd1,cmd2);
                    fflush(stdout);
                    break;
            }
            break;
        case COMMAND_AZIMUTHAL_MOTOR_SET_MAX_SPEED_OPCODE:
//            	printf("MinionSimThread::process_command() : COMMAND_AZIMUTHAL_MOTOR_SET_MAX_SPEED_OPCODE\n");fflush(stdout);
        	mPositionMotorSim->set_speed_rpm(double(cmd1));
            break;
        case COMMAND_AZIMUTHAL_MOTOR_SET_HOME_OPCODE:
//            	printf("MinionSimThread::process_command() : COMMAND_AZIMUTHAL_MOTOR_SET_HOME_OPCODE\n");fflush(stdout);
        	mPositionMotorSim->set_home();
            break;
        case COMMAND_AZIMUTHAL_MOTOR_GO_HOME_OPCODE:
//            	printf("MinionSimThread::process_command() : COMMAND_AZIMUTHAL_MOTOR_GO_HOME_OPCODE\n");fflush(stdout);
        	mPositionMotorSim->homing();
            break;
        case COMMAND_SHUTDOWN_OPCODE:
            break;
        case COMMAND_REBOOT_OPCODE:
            break;
        case COMMAND_CHANGE_TELEMETRY_IP_ADDRESS_AND_UDP_PORT_OPCODE:
            break;
        case COMMAND_ENABLE_DEBUG_LOGGING_OPCODE:
            break;
        default:
            printf("MinionSimThread::process_command() error %s %d %d %d\n",mName.c_str(),cmd0,cmd1,cmd2);
            fflush(stdout);
            break;
    } // switch(cmd0)
}


void MinionSimThread::process_data(char *data,ssize_t nRecv) {
	char dataType[64];
	sscanf(data,"%s",dataType);
	string dataStrType=dataType;
	if(dataStrType=="LIN_POS")
		mVehPos.sread(data);
	else if(dataStrType=="ANG_POS")
		mVehAttitude.sread(data);
	else if(dataStrType=="LIN_VEL")
		mVehLinVel.sread(data);
	else if(dataStrType=="ANG_VEL")
		mVehAngVel.sread(data);
	else if(dataStrType=="FORCE_MOMENT")
		; // do nothing
	else {
		printf("MinionSimThread::process_data unexpected data type %s\n%s\n",dataStrType.c_str(),data);
		fflush(stdout);
	}
}

void MinionSimThread::execute_thread_semantics(void) {
	double thrust,azimuthDeg;
//	printf("MinionSimThread::execute_thread_semantics() %s %" PRIu64 "\n",mName.c_str(),mElapsedMilliseconds);fflush(stdout);
	//
	// compute Minion pose
	//
	compute_device_pose(mMinionPos,mMinionAttitude,mMinionLinVel,mMinionAngVel,
			mPose,
			mVehPos,mVehAttitude,mVehLinVel,mVehAngVel);
	//
	// pump motor simulation step
	//
	mSpeedMotorSim->execute_step(mElapsedMilliseconds);
	//
	// azimuth motor simulation step
	//
	mPositionMotorSim->execute_step(mElapsedMilliseconds);

	//
	// get pumpjet thrust
	//
	thrust=mSpeedMotorSim->get_thrust();
	//
	// get azimuth angle
	//
	azimuthDeg=mPositionMotorSim->get_angle();
//	printf("MinionSim::execute_step %" PRIu64 " thrust %.2lf angle %.2lf\n",elapsedTime,mThrust,mAngleDeg);

	//
	// get GPS data
	//
	mGnssSim->get_measurement(mGnssPosData,mGnssVelData,mMinionPos,mMinionLinVel,mElapsedMilliseconds);
	//
	// get IMU data
	//
	mAhrsSim->get_measurement(mAhrsData,mMinionAttitude,mMinionLinVel,mMinionAngVel,mElapsedMilliseconds);
	//
	// get pump motor data
	//
	mSpeedMotorSim->get_measurement(mSpeedMotorData, mElapsedMilliseconds);
	//
	// get azimuth motor data
	//
	mPositionMotorSim->get_measurement(mPositionMotorData, mElapsedMilliseconds);

	//
	// build force and torque command and append output to mCmdSendBuffer to rigid-body
	//
	build_force_torque_cmd(thrust,azimuthDeg);
//	//
//	// build data packet and append output to mDataSendBuffer
//	//
//	build_data();
}

void MinionSimThread::build_force_torque_cmd(double thrust,double azimuthDeg) {
	//
	// compute force & torque in the RB-fixed reference frame
	//
	double psiThrust=summod360(mPose.psi,azimuthDeg);

	Matrix3x3 R_BD;
	phi_theta_psi_2_Rzyx(R_BD,mPose.phi,mPose.theta,psiThrust);
	Vector force(thrust,0.,0.);
	Vector p(mPose.x,mPose.y,mPose.z);
//	Vector F,T;
	F=R_BD*force;
	T=p%F;
//	printf("MinionSimThread::build_force_torque_cmd - R_BD - psi %lf psiThrust %lf\n",mPose.psi,psiThrust); R_BD.print();
//	printf("MinionSimThread::build_force_torque_cmd - F\n"); F.print();
//	printf("MinionSimThread::build_force_torque_cmd - T\n"); T.print();
}


void MinionSimThread::build_commands(void) {
	//
	// build packet: force & moment in the RB-fixed reference frame
	//
	mRbCmd.set(F(0), F(1), F(2), T(0), T(1), T(2));
	//
	// append packet to cmdSendBuffer
	//
	Packet packet;
	packet.n=mRbCmd.code_set_force_moment((char*)packet.data);
	mCmdSendBuffer[0].push(packet); // there is only one rigid body connected to the minion simulator
//	printf("MinionSimThread::build_commands %s",packet.data);

}


void MinionSimThread::build_data(void) {
	if((mElapsedMilliseconds%mSamplingTime_ms)!=0)
		return;
	//
	// create packet
	//
//	char packet[PACKET_SIZE];
	Packet packet;
	time_t s; int32_t ms;
	get_time_with_ms(s,ms);
	packet.n=sprintf((char*)packet.data,
			// minion Id
			"%d "
			// dateTime , timeMs
			"%" PRIu32 " %" PRIu32 " "
			// plant variables: digital I/O & battery voltage
			"%" PRIu16 " %" PRIu32 " %.2lf "
			// Thrust Motor temperature, speed, current
			"%.2lf %.2lf %.2lf "
			// Azimuth Motor position, configuration status, operation status
			"%" PRId32 " %" PRIu16 " %" PRIu32 " "
			// Azimuth Motor temperature, motor current
			"%" PRId16 " %" PRId32 " "
			// IMU yaw, pitch, roll
			"%.2lf %.2lf %.2lf "
			// IMU yaw, pitch, roll rate
			"%.3lf %.3lf %.3lf "
			// IMU temperature, calibration status
			"%.1lf %" PRIu8 " "
			// GPS date and time
			"%06" PRIu32 " %.2lf "
			// GPS latitute and longitude
			"%.8lf %.8lf "
			// GPS fix quality, n satellites, HDOP
			"%" PRIu8 " %02" PRIu8 " %.2lf "
			// GPS altutude, height geoid
			"%.1lf %.1lf "
			"\r\n",
			// minion Id
			mId,
			// dateTime , timeMs
			(uint32_t)s,(uint32_t)ms,
			// plant variables: digital I/O
			mDigitalOutput,mDigitalInput,mBatteryVoltage,
			// Thrust Motor temperature, speed, current
//			mSpeedMotorData.temperature,mSpeedMotorData.speed/THRUST_SPEED_PERC_FACTOR,mSpeedMotorData.currentVolt,
			mSpeedMotorData.temperature,mSpeedMotorData.speed/THRUST_SPEED_PERC_FACTOR,mSpeedMotorData.current,
			// Azimuth Motor position, configuration status, operation status
//			mPositionMotorData.position,mPositionMotorData.configurationStatus,mPositionMotorData.operationStatus,
			mPositionMotorData.position,mPositionMotorSim->mConfigurationStatus,0,
			// Azimuth Motor temperature, motor current
			(int16_t)mPositionMotorData.temperature,(int32_t)mPositionMotorData.current,
			// IMU yaw, pitch, roll
			mAhrsData.psi,mAhrsData.theta,mAhrsData.phi,
			// IMU yaw, pitch, roll rate
			mAhrsData.r,mAhrsData.q,mAhrsData.p,
			// IMU temperature, calibration status
			//mAhrsData.temperature,mAhrsData.calibrationStatus,
			40.0,0,
			// GPS date and time
			(uint32_t)mGnssPosData.date,mGnssPosData.time,
			// GPS latitute and longitude
			mGnssPosData.latitude,mGnssPosData.longitude,
			// GPS fix quality, n satellites, HDOP
			mGnssPosData.fixQuality,mGnssPosData.nSatellites,mGnssPosData.hdop,
			// GPS altutude, height geoid
			mGnssPosData.heightAboveMSL,mGnssPosData.heightAboveEllipsoid
			);

	mDataSendBuffer.push(packet);

//	printf("MinionSimThread::build_data: %s packet %s\n",mName.c_str(),(char*)packet.data);

}




