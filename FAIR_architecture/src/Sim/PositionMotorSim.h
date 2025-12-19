//
//  PositionMotorSim.h
//  PositionMotor simulator
//
//  Created by Massimo Caccia on 15/03/23.
//

#ifndef PositionMotorSim_h
#define PositionMotorSim_h

#include "PositionMotorData.h"
#include "NormalNoiseGenerator.h"
#include "MathAngles.h"

//
// define standard PositionMotor parameters
//
#define POSITION_MOTOR_MAX_RPM 4500 // [rpm]
#define POSITION_MOTOR_TICKS_PER_MOTOR_REV 4096 // speed[tick/s] = speed[rpm]*TICKS_PER_MOTOR_REV/60
#define TICKS_PER_360DEG_MOTOR_REV 939804.444444445   // deg=(360/TICKS_PER_REVOLUTION)*ticks
#define POSITION_MOTOR_TEMPERATURE 37.0

#define POSITION_RPM2CURRENT 1.0

//
// define maximum number of position motor ARMA model parameters
//
#define N_POS_MOTOR_ARMA_PAR 5

//
// define standard SpeedMotor measurement errors
//
#define POSITION_MOTOR_TICK_NOISE_MEAN 0.
#define POSITION_MOTOR_TICK_NOISE_STD 0.5
#define POSITION_MOTOR_CURRENT_NOISE_MEAN 0.
#define POSITION_MOTOR_CURRENT_NOISE_STD 0.5
#define POSITION_MOTOR_TEMPERATURE_NOISE_MEAN 0.
#define POSITION_MOTOR_TEMPERATURE_NOISE_STD 0.5

//
// define standard position motor sampling time [ms]
//
#define POSITION_MOTOR_SAMPLING_TIME 1000

enum PositionMotorMode
{
	POSITION_MOTOR_HOMING=0,
	POSITION_MOTOR_GOTO
};

class PositionMotorSim {
public:
	PositionMotorSim(void);
    ~PositionMotorSim(void);

    void set_power(bool value);
    void set_enable(bool value);
    void set_speed_rpm(double speedRpmRef);
    void set_position(int32_t positionRef);
    void set_home(void);
    void homing(void);

    void get_measurement(PositionMotorData &positionMotorData,uint64_t elapsedTime);
    double get_angle(void);

    bool read_parameters(FILE *f);

    void execute_step(uint64_t elapsedTime);

private:
    void set_parameters(double maxSpeedRpm,double rpm2current,
    		double noisePositionTickMean,double noisePositionTickStd,
    		double noiseCurrentMean,double noiseCurrentStd,
    		double noiseTemperatureMean,double noiseTemperatureStd);

private:
//    string mName;
    uint64_t mSamplingTime;

    bool mPower;
    bool mEnable;
    int32_t mPosition; // position [tick]
    double mTemperature;

    double mMaxSpeed,mMaxSpeedRpm;  // max speed [tick/s] [rpm]
    double mRpm2current;


    double mARMA_a[N_POS_MOTOR_ARMA_PAR];// Note: mARMA_a[0]=0.;
    double mARMA_b[N_POS_MOTOR_ARMA_PAR];
    double mPosARMA[N_POS_MOTOR_ARMA_PAR];
    double mPosRefARMA[N_POS_MOTOR_ARMA_PAR];


    int32_t mPositionRef; // reference position [tick]

    PositionMotorMode mMode;

//    uint64_t m_elapsed_time;
    double mNoisePositionTickMean,mNoisePositionTickStd; // position noise in [tick]
    double mNoiseCurrentMean,mNoiseCurrentStd; // speed motor measured current noise in [V]
    double mNoiseTemperatureMean,mNoiseTemperatureStd; // speed motor temperature noise in [C]
    //
    // normal noise generators
    //
    NormalNoiseGenerator *mNormalPositionNoiseGenerator;
    NormalNoiseGenerator *mNormalCurrentNoiseGenerator;
    NormalNoiseGenerator *mNormalTemperatureNoiseGenerator;

    PositionMotorData mPositionMotorData;
public:
    uint16_t mConfigurationStatus;
};


#endif /* PositionMotorSim_h */
