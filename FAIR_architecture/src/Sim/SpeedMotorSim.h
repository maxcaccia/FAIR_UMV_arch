//
//  SpeedMotorSim.h
//  SpeedMotor simulator
//
//  Created by Massimo Caccia on 02/03/23.
//

#ifndef SPEEDMOTORSIM_H
#define SPEEDMOTORSIM_H

#include "SpeedMotorData.h"
#include "NormalNoiseGenerator.h"

//
// define standard SpeedMotor parameters
// 	bilinear model : y = k2*n^2 + k0
//
#define SPEED_MOTOR_RPM2THRUST2		0.0000033
#define SPEED_MOTOR_RPM2THRUST0 	0.0
#define SPEED_MOTOR_RPM2CURRENT2 	0.0000019
#define SPEED_MOTOR_RPM2CURRENT0 	0.49
#define SPEED_MOTOR_TEMPERATURE 	37.0

//
// define maximum number of speed motor ARMA model parameters
//
#define N_SPEED_MOTOR_ARMA_PAR 5

//
// define standard SpeedMotor measurement errors
//
#define SPEED_MOTOR_RPM_NOISE_MEAN 0.
#define SPEED_MOTOR_RPM_NOISE_STD 0.5
#define SPEED_MOTOR_CURRENT_NOISE_MEAN 0.
#define SPEED_MOTOR_CURRENT_NOISE_STD 0.5
#define SPEED_MOTOR_TEMPERATURE_NOISE_MEAN 0.
#define SPEED_MOTOR_TEMPERATURE_NOISE_STD 0.5

//
// define standard speed motor sampling time [ms]
//
#define SPEED_MOTOR_SAMPLING_TIME 1000


class SpeedMotorSim {
public:
	SpeedMotorSim(void);
    ~SpeedMotorSim(void);

    void set_power(bool value);
    void set_enable(bool value);
    void set_speed(double speedRef);

    void get_measurement(SpeedMotorData &speedMotorData,uint64_t elapsedTime);
    double get_thrust(void);

    bool read_parameters(FILE *f);

    void execute_step(uint64_t elapsedTime);

private:
    void set_parameters(double rpm2thrust2,double rpm2thrust0,double rpm2current2,double rpm2current0,
    		double noiseRpmMean,double noiseRpmStd,
    		double noiseCurrentMean,double noiseCurrentStd,
    		double noiseTemperatureMean,double noiseTemperatureStd);
    void set_ARMA_model_parameters(double *a,double *b) {for(int i=0;i<N_SPEED_MOTOR_ARMA_PAR;i++){mARMA_a[i]=a[i];mARMA_b[i]=b[i];}}

private:
//    string mName;
    uint64_t mSamplingTime;

    bool mPower;
    bool mEnable;
    double mSpeedRef;
    double mSpeed;
    double mTemperature;

    //
    // bilinear model : y = k2*n^2 + k0
    //
    double mRpm2thrust2;
    double mRpm2thrust0;
    double mRpm2current2;
    double mRpm2current0;

    double mARMA_a[N_SPEED_MOTOR_ARMA_PAR];// Note: mARMA_a[0]=0.;
    double mARMA_b[N_SPEED_MOTOR_ARMA_PAR];
    double mSpeedARMA[N_SPEED_MOTOR_ARMA_PAR];
    double mSpeedRefARMA[N_SPEED_MOTOR_ARMA_PAR];

//    uint64_t m_elapsed_time;
    double mNoiseRpmMean,mNoiseRpmStd; // speed noise in [rpm]
    double mNoiseCurrentMean,mNoiseCurrentStd; // speed motor measured current noise in [V]
    double mNoiseTemperatureMean,mNoiseTemperatureStd; // speed motor temperature noise in [C]
    //
    // normal noise generators
    //
    NormalNoiseGenerator *mNormalRpmNoiseGenerator;
    NormalNoiseGenerator *mNormalCurrentNoiseGenerator;
    NormalNoiseGenerator *mNormalTemperatureNoiseGenerator;

    SpeedMotorData mSpeedMotorData;
};


#endif /* SPEEDMOTORSIM_H */
