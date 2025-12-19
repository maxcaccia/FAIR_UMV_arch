/*
 * P2LSM.h
 *
 *  Created on:13 Jan 2025
 *      Author: massimo
 */

#ifndef P2LSM_H_
#define P2LSM_H_

#include <stdio.h>
#include <vector>

//#include <inttypes.h>
//#include "SensorInfo.h"

#include "SensorInfo.h"
#include "AhrsData.h"
#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "DvlData.h"
#include "LogicalPositionData.h"
#include "LogicalAttitudeData.h"
#include "LogicalSpeedData.h"
#include "LogicalLinearRateData.h"
#include "LogicalAngularRateData.h"
#include "P2LSMstate.h"

#define LOWEST_PRIORITY	255	// highest priority is 0

#define SENSOR_TIMEOUT	5.0		// time in seconds

class P2LSM {
private:
	string mName;
	string mConfigDir;
    ////////////
    //
    // sensor information (name, pose, measurement uncertainty)
    //
//    SensorInfo *mSensorInfo;
//    int *nSensor;
	vector<SensorInfo> *mSensorInfo;

    ////////////
    //
    // Driver Interface - sensor data: AHRS, GNSS-GGA, GNSS-VTG, DVL
    //
    AhrsData *mAhrsData;
    GnssPositionData *mGnssPositionData;
    GnssVelocityData *mGnssVelocityData;
    DvlData *mDvlData;
    ////////////
    //
    // Logical Sensors: Physical Sensor priorities
    //
    vector<string> *mLogicalPosition;
    vector<string> *mLogicalAttitude;
    vector<string> *mLogicalSpeed;
    vector<string> *mLogicalLinearRate;
    vector<string> *mLogicalAngularRate;
    ////////////
    //
    // NGC interface - Logical data
    //
    LogicalPositionData *mLogicalPositionData;
    LogicalAttitudeData *mLogicalAttitudeData;
    LogicalSpeedData *mLogicalSpeedData;
    LogicalLinearRateData *mLogicalLinearRateData;
    LogicalAngularRateData *mLogicalAngularRateData;

    double* mTimeStamp;
    double dt;

private:
    //
    // P2LSM state : active logical sensors
    //
    P2LSMstate *mP2LSMstate;
//    //
//    // active Logical Sensors
//    //
//    string mActiveLogicalPosition;
//    string mActiveLogicalAttitude;
//    string mActiveLogicalSpeed;
//    string mActiveLogicalLinearRate;
//    string mActiveLogicalAngularRate;
    //
    // elapsed time for each Logical Sensor
    //
    uint64_t mElapsedTimeLogicalPosition;
    uint64_t mElapsedTimeLogicalAttitude;
    uint64_t mElapsedTimeLogicalSpeed;
    uint64_t mElapsedTimeLogicalLinearRate;
    uint64_t mElapsedTimeLogicalAngularRate;
    //
    // priority for each active Logical Sensor
    //
    uint8_t mPriorityLogicalPosition;
    uint8_t mPriorityLogicalAttitude;
    uint8_t mPriorityLogicalSpeed;
    uint8_t mPriorityLogicalLinearRate;
    uint8_t mPriorityLogicalAngularRate;

public:
	// constructor/destructor
    P2LSM();
    ~P2LSM(void);
    void reset(void);
    void set_name(string name) { mName=name; }
    void set_config_dir(string configDir) { mConfigDir=configDir; }
    // configure
//    void set_sensor_info(SensorInfo* sensorInfo,int *nSensor) {
//    	mSensorInfo=sensorInfo;
//    	this->nSensor=nSensor;
//    }
    void set_sensor_info(vector<SensorInfo>* sensorInfo) {
    	mSensorInfo=sensorInfo;
//    	this->nSensor=nSensor;
    }

    void set_driver_interface(AhrsData* ahrsData,
    		GnssPositionData* gnssPositionData,
			GnssVelocityData* gnssVelocityData,
			DvlData* dvlData) {
    	mAhrsData=ahrsData;
    	mGnssPositionData=gnssPositionData;
    	mGnssVelocityData=gnssVelocityData;
    	mDvlData=dvlData;
    }
    void set_sensor_priorities(vector<string>* logicalPosition,
    		vector<string>* logicalAttitude,
			vector<string>* logicalSpeed,
			vector<string>* logicalLinearRate,
			vector<string>* logicalAngularRate) {
    	mLogicalPosition=logicalPosition;
    	mLogicalAttitude=logicalAttitude;
    	mLogicalSpeed=logicalSpeed;
    	mLogicalLinearRate=logicalLinearRate;
    	mLogicalAngularRate=logicalAngularRate;
    }
    void set_ngc_interface(LogicalPositionData* logicalPositionData,
    		LogicalAttitudeData* logicalAttitudeData,
    		LogicalSpeedData* logicalSpeedData,
			LogicalLinearRateData* logicalLinearRateData,
			LogicalAngularRateData* logicalAngularRateData) {
    	mLogicalPositionData=logicalPositionData;
    	mLogicalAttitudeData=logicalAttitudeData;
    	mLogicalSpeedData=logicalSpeedData;
    	mLogicalLinearRateData=logicalLinearRateData;
    	mLogicalAngularRateData=logicalAngularRateData;
    }
    void set_state(P2LSMstate* state) {
    	mP2LSMstate=state;
    }

    void set_dt(double dt) { this->dt = dt; }


 //    void set_sensor_info(SensorInfo *forceTorqueInfo,SensorInfo *gnssBodyInfo,SensorInfo *gnssSternInfo,SensorInfo* gnssBowInfo,SensorInfo* ahrsBodyInfo)
 //   { mForceTorqueInfo=forceTorqueInfo;mGnssBodyInfo=gnssBodyInfo,mGnssSternInfo=gnssSternInfo,mGnssBowInfo=gnssBowInfo,mAhrsBodyInfo=ahrsBodyInfo; }

    void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
    bool configure(void);
    void process_command(char *cmd,ssize_t n);
    bool process_data(char *data,ssize_t nRecv);
   // execute
	void execute(void);

	bool read_configuration(void);
	void reset_state(void) { mP2LSMstate->reset(); }

private:
    bool read_priorities(FILE *f);
    bool read_sensor_list(FILE *f);
    bool read_sensor_block(FILE *f,string &strDev,SensorInfo &sensorInfo);
    bool read_priority_list(FILE *f,vector<string> &list,string name);

    void print_sensor_list(void);
    void print_configuration(void);
    void print_list(vector<string> list);

    bool is_in_sensor_list(string s);
    uint8_t get_priority(string name,vector<string> list);
    bool get_sensor_info(SensorInfo &sensorInfo,string name);

    bool process_ahrs_data(void);
    bool process_gnss_gga_data(void);
    bool process_gnss_vtg_data(void);
    bool process_dvl_data(void);
};

#endif /* P2LSM_H_ */
