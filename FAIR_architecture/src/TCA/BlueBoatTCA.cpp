/*
 * BlueBoatTCA.cpp
 *
 *  Created on: 28 June 2025
 *      Author: massimo
 */

#include "BlueBoatTCA.h"
#include "MathAngles.h"
#include "BasicMath.h"
#include "Packet.h"

BlueBoatTCA::BlueBoatTCA(void):TCA()
{
	mBlueBoatRpmRef=NULL;
	mBlueBoatRpmMeas=NULL;

//	mTCAstate->mMode = TCA_MANUAL;
//	printf("BlueBoatTCA::SwampTCA mMode %s\n",mTCAstate->mMode.c_str());fflush(stdout);

//	mTCAstate->mMode = TCA_MANUAL;
//	//
//	// zeroes force/torque references
//	//
//	mForceMomentRef->reset();
//	//
//	// set default AUTO mode
//	//
//	set_auto_default();

}

BlueBoatTCA::~BlueBoatTCA(void) {
	TCA::~TCA();
	if(mBlueBoatRpmRef!=NULL) {
		delete mBlueBoatRpmRef;
		mBlueBoatRpmRef=NULL;
	}
	if(mBlueBoatRpmMeas!=NULL) {
		delete mBlueBoatRpmMeas;
		mBlueBoatRpmMeas=NULL;
	}
}

void BlueBoatTCA::set_input_output(NormalisedRpm* blueBoatRpmRef,NormalisedRpm* blueBoatRpmMeas) {
	mBlueBoatRpmRef=blueBoatRpmRef;
	mBlueBoatRpmMeas=blueBoatRpmMeas;
	BlueBoatTCA::reset();
}


void BlueBoatTCA::process_command(char *cmd,ssize_t n) {
	TCA::process_command(cmd,n);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType=="BLUEBOAT_TCA") {
		if(mBlueBoatTcaCmd.decode(cmd, strCmd)) {
			if(strCmd=="SET_LEFT_RIGHT_RPM") {
				mBlueBoatRpmRef->mTimeStamp = 0;
				process_set_left_right_rpm();
			}
			if(strCmd=="SET_DIFFERENTIAL_RPM") {
				mBlueBoatRpmRef->mTimeStamp = 0;
				process_set_differential_rpm();
			}
			else if(strCmd=="SET_MANUAL_MODE") { // set default BlueBoat manual mode
				process_set_manual_mode();
			}
			else if(strCmd=="SET_GUIDED_MODE") { // set default BlueBoat guided mode
				process_set_guided_mode();
			}
			else if(strCmd=="ARM") { // set motors armed
				process_arm();
			}
			else if(strCmd=="DISARM") { // set motors disarmed
				process_disarm();
			}
			else if(strCmd=="MAP_SERVO_DEFAULT") { // set map servo default
				process_map_servo_default();
			}
			else if(strCmd=="MAP_SERVO_RAW") { // set map servo raw
				process_map_servo_raw();
			}
			else {
//				printf("BlueBoatTCA::decode ERROR read - BLUEBOAT_TCA cmd is %s\n",strCmd.c_str());
//				fflush(stdout);
				return ;
			}
		}
	} // if(strType=="BLUEBOAT_TCA")
	else  {
//		printf("BlueBoatTCA::process_command ERROR read - type is %s\n",strType.c_str());
//		fflush(stdout);
		return ;
	}
}


bool BlueBoatTCA::process_data(char *data,ssize_t nRecv) {
	//	printf("BlueBoatTCAthread::process_data nRecv %d data %s\n",(int)nRecv,data);
		char dataType[64];
		sscanf(data,"%s",dataType);
		string dataStrType=dataType;
		if(dataStrType=="NORM_RPM") {
//			mBlueBoatRpmMeas->mTimeStamp = mTimeStamp;
			mBlueBoatRpmMeas->sread(data);
			mBlueBoatRpmMeas->set_name("msr_"+mName);
			
			//char s[512];
			//mBlueBoatRpmMeas->sprint(s);
			//printf("BlueBoatTCA::process_data %s\n",s);
			//fflush(stdout);
		}
		else {
//			printf("BlueBoatTCA::process_data unexpected data type %s\n",dataStrType.c_str());
//			fflush(stdout);
			return false;
		}
	return true;
}


void BlueBoatTCA::process_set_left_right_rpm(void) {
	// set left & right rpm
	// TCA mode is automatically set to raw
	set_raw_default();
	set_left_right_rpm(mBlueBoatTcaCmd.nL,mBlueBoatTcaCmd.nR);
}


void BlueBoatTCA::process_set_differential_rpm(void) {
	//printf("BlueBoatTCA::process_set_differential_rpm n %.3lf dn %.3lf\n",mBlueBoatTcaCmd.n,mBlueBoatTcaCmd.delta_n);
	//fflush(stdout);
	// set differential rpm
	// TCA mode is automatically set to manual
	set_manual_default();
	set_differential_rpm(mBlueBoatTcaCmd.n,mBlueBoatTcaCmd.delta_n);
}

void BlueBoatTCA::process_set_manual_mode(void) {
	// set default manual mode
	Packet packet;
	BlueBoatCmd blueBoatCmd;
	blueBoatCmd.mode=BLUE_BOAT_MANUAL;
	packet.n=blueBoatCmd.code_set_mode((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
}


void BlueBoatTCA::process_set_guided_mode(void) {
printf("BlueBoatTCA::process_set_guided_mode\n");fflush(stdout);
	// set default manual mode
	Packet packet;
	BlueBoatCmd blueBoatCmd;
	blueBoatCmd.mode=BLUE_BOAT_GUIDED;
	packet.n=blueBoatCmd.code_set_mode((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
}


void BlueBoatTCA::process_arm(void) {
	// set motors armed
	Packet packet;
	BlueBoatCmd blueBoatCmd;
	blueBoatCmd.arm=BLUE_BOAT_ARM;
	packet.n=blueBoatCmd.code_set_arm((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
}

void BlueBoatTCA::process_disarm(void) {
	// set motors disarmed
	Packet packet;
	BlueBoatCmd blueBoatCmd;
	blueBoatCmd.arm=BLUE_BOAT_DISARM;
	packet.n=blueBoatCmd.code_set_arm((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
}

void BlueBoatTCA::process_map_servo_default(void) {
	// set map servo default
	Packet packet;
	BlueBoatCmd blueBoatCmd;
	packet.n=blueBoatCmd.code_map_servo_default((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
}

void BlueBoatTCA::process_map_servo_raw(void) {
	// set map servo raw
	Packet packet;
	BlueBoatCmd blueBoatCmd;
	packet.n=blueBoatCmd.code_map_servo_raw((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
}


void BlueBoatTCA::set_raw_default(void) {
	//
	// set TCA default RAW mode
	//
	mTCAstate->mMode="raw";
	mTCAstate->mFunction="steering";
	mTCAstate->mConfig="surge_yaw";
}

void BlueBoatTCA::set_manual_default(void) {
	//
	// set TCA default MANUAL mode
	//
	mTCAstate->mMode="manual";
	mTCAstate->mFunction="steering";
	mTCAstate->mConfig="surge_yaw";
}

void BlueBoatTCA::set_auto_default(void) {
	//
	// set default AUTO mode
	//
	mTCAstate->mMode="auto";
	mTCAstate->mFunction="steering";
	mTCAstate->mConfig="surge_yaw";
}


void BlueBoatTCA::reset(void) {
//printf("BlueBoatTCA::reset %llu\n",(unsigned long long)mSwampActuationRef);fflush(stdout);
	TCA::reset();
	mBlueBoatRpmRef->reset();
}


//void BlueBoatTCA::set_mode(string  str) {
////printf("BlueBoatTCA::set_mode\n");fflush(stdout);
//	TCA::set_mode(str);
//	if(mTCAstate->mMode=="manual") {
//		//
//		// zeroes force/torque references
//		//
//		mForceMomentRef->reset();
//		//
//		// zeroes BlueBoat references
//		//
//		mBlueBoatRpmRef->reset();
//		//
//		// set default MANUAL mode
//		//
//		set_manual_default();
////		mTCAstate->mFunction="forward_steering";
////		mTCAstate->mConfig="all";
//	}
//	else if(mTCAstate->mMode=="auto") {
//		//
//		// zeroes force/torque references
//		//
//		mForceMomentRef->reset();
//		//
//		// set default AUTO mode
//		//
//		set_auto_default();
////		mTCAstate->mFunction="steering";
////		mTCAstate->mConfig="all";
//	}
//	else {
//		//
//		// zeroes force/torque references
//		//
//		mForceMomentRef->reset();
//		//
//		// zeroes BlueBoat references
//		//
//		mBlueBoatRpmRef->reset();
//		//
//		// set default MANUAL mode
//		//
//		set_manual_default();
//		//
//		// printf error message
//		//
//		printf("BlueBoatTCA::set_mode - unexpected mode %s\n",str.c_str());
//		fflush(stdout);
//	}
//}


void BlueBoatTCA::set_left_right_rpm(double nL,double nR) {
	mBlueBoatRpmRef->mLeftRpm = sat(nL,-100.,100.);
	mBlueBoatRpmRef->mRightRpm = sat(nR,-100.,100.);
}

void BlueBoatTCA::set_differential_rpm(double n,double delta_n) {
	//printf("BlueBoatTCA::set_differential_rpm n %.3lf dn %.3lf\n",mBlueBoatTcaCmd.n,mBlueBoatTcaCmd.delta_n);
	//fflush(stdout);
	mBlueBoatRpmRef->mLeftRpm = sat(n+delta_n,-100.,100.);
	mBlueBoatRpmRef->mRightRpm = sat(n-delta_n,-100.,100.);
	//printf("BlueBoatTCA::set_differential_rpm rpmLeft %.3lf rpmRight %.3lf\n",
		//mBlueBoatRpmRef->mLeftRpm,mBlueBoatRpmRef->mRightRpm);
	//fflush(stdout);
}


void BlueBoatTCA::compute_applied_force_torque(void) {
//	printf("BlueBoatTCA::compute_applied_force_torque : to be implemented!\n");
	fflush(stdout);
	double TL,TR;

	//
	// compute TL,TR
	//
	TL = compute_propeller_thrust(1.,kDiff,mBlueBoatRpmMeas->mLeftRpm/100.);
	TR = compute_propeller_thrust(1.,kDiff,mBlueBoatRpmMeas->mRightRpm/100.);

	double f=TL+TR;
	mXYNnormHat->X=100.*f/2.;
	mXYNnormHat->N=100*(TL-TR)/(1+kDiff);

	normalised_to_standard_hat();
}


void BlueBoatTCA::execute_raw(void) {
//	printf("BlueBoatTCA::execute_raw left %.3lf right %.3lf\n",mBlueBoatRpmRef->mLeftRpm,mBlueBoatRpmRef->mRightRpm);fflush(stdout);
}

void BlueBoatTCA::execute_manual(void) {

}


void BlueBoatTCA::execute_auto(void) {
//	printf("BlueBoatTCA::execute_auto\n");fflush(stdout);
	TCA::execute_auto();
	TCA_BlueBoat();
}

void BlueBoatTCA::handle_nop_emergency(void) {
	printf("******************************************************************* BlueBoatTCA::handle_nop_emergency\n");
	fflush(stdout);

	stop();
	mNopHandlerEnable = false;

}


void BlueBoatTCA::start(void) {
	process_set_manual_mode();
	process_arm();
	process_map_servo_raw();
	set_manual_default();
	set_differential_rpm(0.,0.);
	mTCAstate->mEnable = true;
}


void BlueBoatTCA::stop(void) {
	//
	// append reference commands to mCmdSendBuffer
	//
	Packet packet;
	//
	// BlueBoat
	// set left and right PWM to 0
	//
	BlueBoatCmd mBlueBoatCmd;
	mBlueBoatCmd.leftRpm = 0;
	mBlueBoatCmd.rightRpm = 0;
	packet.n=mBlueBoatCmd.code_set_manual_ref((char*)packet.data);
	mCmdSendBuffer[0].push(packet);
//	printf("******************************************************************* BlueBoatTCA::stop: %s packet %s",mName.c_str(),(char*)packet.data);

	mTCAstate->mEnable = false;

	process_map_servo_default();
	process_set_manual_mode();
//	process_disarm();
	
//	process_arm();
	process_set_guided_mode();


}


//bool BlueBoatTCA::read_lookup_tables(void) {
//	string filename;
//	filename=mName+"LeftLookupTable.txt";
//	bool flag;
//	flag=load_lookup_table(filename.c_str(), mLeftLookupTable);
//	if(!flag) {
//		printf("BlueBoatTCA::read_lookup_tables Error loading file %s\n",filename.c_str());fflush(stdout);
//		return false;
//	}
//	filename=mName+"RightLookupTable.txt";
//	flag=load_lookup_table(filename.c_str(), mRightLookupTable);
//	if(!flag) {
//		printf("BlueBoatTCA::read_lookup_tables Error loading file %s\n",filename.c_str());fflush(stdout);
//		return false;
//	}
//	return true;
//}
//
//
//bool BlueBoatTCA::load_lookup_table(const char *filename, double table[TABLE_SIZE][TABLE_SIZE]) {
//    FILE *file = fopen(filename, "r");
//    if (!file) {
//        printf("BlueBoatTCA::load_lookup_table Error opening file\n");fflush(stdout);
//        return false;
//    }
//
//    for (int i = 0; i < TABLE_SIZE; ++i) {
//        for (int j = 0; j < TABLE_SIZE; ++j) {
//            if (fscanf(file, "%lf", &table[i][j]) != 1) {
//            	printf("BlueBoatTCA::load_lookup_table Error reading value at [%d][%d]\n", i, j);
//                fflush(stdout);
//                fclose(file);
//                return false;
//            }
//        }
//    }
//
//    fclose(file);
//    return true;
//}


void BlueBoatTCA::TCA_BlueBoat(void) {
	double Tleft = mThrustNorm[0];
	double Tright = mThrustNorm[1];

	mBlueBoatRpmRef->mLeftRpm = 100.*compute_propeller_rpm(1.,kDiff,Tleft);
	mBlueBoatRpmRef->mRightRpm = 100.*compute_propeller_rpm(1.,kDiff,Tright);
		
	//printf("BlueBoatTCA::TCA_BlueBoat rpmLeft %.3lf rpmRight %.3lf\n",mBlueBoatRpmRef->mLeftRpm,mBlueBoatRpmRef->mRightRpm);
	//fflush(stdout);
}


