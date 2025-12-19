/*
 * P2LSMthread.h
 *
 *  Created on: Jan 13, 2025
 *      Author: mc
 *
 *  P2LSMthread is the parent class for <Robot>P2LSMthread
 */

#ifndef P2LSMTHREAD_H_
#define P2LSMTHREAD_H_

#include "CntrlSimThread.h"
#include "P2LSM.h"

//#include <list>

class P2LSMthread: public CntrlSimThread {
public:
	P2LSMthread();
	virtual ~P2LSMthread();

	void make_thread(void);

	bool init_configuration(void);

protected:
	void execute(void) {CntrlSimThread::execute();}
    void process_command(char *cmd,ssize_t n); // { printf("NGCthread::process_command() %s\n",mName.c_str()); } // function to be overwritten
    void process_data(char *data,ssize_t nRecv); // { printf("NGCthread::process_data() %s %ld : %s\n",mName.c_str(),nRecv,data); } // function to be overwritten
    void execute_thread_semantics(void) { /*printf("NGCthread::execute_thread_semantics() %s\n",mName.c_str());*/ mP2LSM->execute();} // function to be overwritten
    void build_data(void);

//private:
//    bool read_priorities(FILE *f);
//    bool read_sensor_list(FILE *f);
//    bool read_sensor_block(FILE *f,string &strDev,SensorInfo &sensorInfo);
//    bool read_priority_list(FILE *f,vector<string> &list,string name);
//
//    void print_sensor_list(void);
//    void print_configuration(void);
//    void print_list(vector<string> list);
//
//    bool is_in_sensor_list(string s);

private:
    //
    // P2LSM state : active logical sensors
    //
    P2LSMstate mP2LSMstate;

    ////////////
    //
    // sensor information (name, pose, measurement uncertainty)
    //
    vector<SensorInfo> mSensorInfo;
//    SensorInfo mSensorInfo[MAX_N_SENSORS];
//    int nSensor;

    ////////////
    //
    // Driver Interface - sensor data: AHRS, GNSS-GGA, GNSS-VTG, DVL
    //
    AhrsData mAhrsData;
    GnssPositionData mGnssPositionData;
    GnssVelocityData mGnssVelocityData;
    DvlData mDvlData;

    ////////////
    //
    // Logical Sensors: Physical Sensor priorities
    //
    vector<string> mLogicalPosition;
    vector<string> mLogicalAttitude;
    vector<string> mLogicalSpeed;
    vector<string> mLogicalLinearRate;
    vector<string> mLogicalAngularRate;

    ////////////
    //
    // NGC interface - Logical data
    //
    LogicalPositionData mLogicalPositionData;
    LogicalAttitudeData mLogicalAttitudeData;
    LogicalSpeedData mLogicalSpeedData;
    LogicalLinearRateData mLogicalLinearRateData;
    LogicalAngularRateData mLogicalAngularRateData;

    ////////////
    //
    // P2LSM module
    //
    P2LSM* mP2LSM;

};



#endif /* NGCTHREAD_H_ */
